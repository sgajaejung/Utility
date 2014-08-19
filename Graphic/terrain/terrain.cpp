
#include "stdafx.h"
#include "terrain.h"

using namespace std;
using namespace Gdiplus;
using namespace graphic;



cTerrain::cTerrain() :
	m_rowCellCount(0)
,	m_colCellCount(0)
,	m_cellSize(0)
,	m_textureUVFactor(1.f)
,	m_heightFactor(3.f)
,	m_modelShader(NULL)
{
	m_rigids.reserve(32);

	m_modelShader = cResourceManager::Get()->LoadShader(  "hlsl_skinning_no_light.fx" );

}

cTerrain::~cTerrain()
{
	Clear();
}


bool cTerrain::CreateFromHeightMap( const string &heightMapFileName, 
	const string &textureFileName, const float heightFactor, const float textureUVFactor,
	const int rowCellCount, const int colCellCount, const float cellSize)
	// heightFactor=3.f, textureUVFactor=1.f
	// rowCellCount=64, colCellCount=64, cellSize=50.f
{
	CreateTerrain(rowCellCount, colCellCount, cellSize, textureUVFactor);
	const bool result = UpdateHeightMap(heightMapFileName, textureFileName, heightFactor );
	return result;
}


bool cTerrain::CreateFromGRDFormat( const string &gridFileName, 
	const string &textureFileName, const float heightFactor, const float textureUVFactor,
	const int rowCellCount, const int colCellCount, const float cellSize)
	// heightFactor=3.f, textureUVFactor=1.f
	// rowCellCount=64, colCellCount=64, cellSize=50.f
{
	CreateTerrain(rowCellCount, colCellCount, cellSize, textureUVFactor);

	if (!m_grid.ReadGridFromFile(gridFileName))
		return false;

	m_grid.CalculateNormals();
	m_grid.GetTexture().Create( textureFileName );

	m_heightFactor = heightFactor;
	m_heightMapFileName = gridFileName;

	return true;
}


// ���� �ؽ��� ����.
bool cTerrain::CreateTerrainTexture( const string &textureFileName )
{
	m_grid.GetTexture().Clear();
	return m_grid.GetTexture().Create( textureFileName );
}


// ���� ����.
bool cTerrain::CreateTerrain( const int rowCellCount, const int colCellCount, const float cellSize
	,const float textureUVFactor)
	// rowCellCount=64, colCellCount=64, cellSize=50.f, textureUVFactor=1.f
{
	Clear();

	m_rowCellCount = rowCellCount;
	m_colCellCount = colCellCount;
	m_cellSize = cellSize;
	m_textureUVFactor = textureUVFactor;
	m_grid.Create(rowCellCount, colCellCount, cellSize, textureUVFactor);

	return true;
}


// �ؽ��� ���� ������ ���� ������ ä���.
// m_grid �� ������ ���¿��� �Ѵ�.
bool cTerrain::UpdateHeightMap( const string &heightMapFileName, 
	const string &textureFileName, const float heightFactor )
{
	m_heightFactor = heightFactor;
	m_heightMapFileName = heightMapFileName;

	const wstring wfileName = common::str2wstr(heightMapFileName);
	Gdiplus::Bitmap bmp(wfileName.c_str());
	if (Gdiplus::Ok != bmp.GetLastStatus())
		return false;

	const int VERTEX_COL_COUNT = m_colCellCount + 1;
	const int VERTEX_ROW_COUNT = m_rowCellCount + 1;
	const float WIDTH = m_colCellCount * m_cellSize;
	const float HEIGHT = m_rowCellCount * m_cellSize;

	const float incX = (float)(bmp.GetWidth()-1) / (float)m_colCellCount;
	const float incY = (float)(bmp.GetHeight()-1) /(float) m_rowCellCount;

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();

	for (int i=0; i < VERTEX_COL_COUNT; ++i)
	{
		for (int k=0; k < VERTEX_ROW_COUNT; ++k)
		{
			sVertexNormTex *vtx = pv + (k*VERTEX_COL_COUNT) + i;

			Gdiplus::Color color;
			bmp.GetPixel((int)(i*incX), (int)(k*incY), &color);
			const float h = ((color.GetR() + color.GetG() + color.GetB()) / 3.f) 
				* heightFactor;
			vtx->p.y = h;
		}
	}

	m_grid.GetVertexBuffer().Unlock();

	m_grid.CalculateNormals();
	m_grid.GetTexture().Create( textureFileName );

	return true;
}



void cTerrain::Render()
{
	m_grid.Render();
}


void cTerrain::RenderShader(cShader &shader)
{
	Vector3 fog(1.f, 10000.f, 0);  // near, far
	shader.SetVector( "vFog", fog);

	m_grid.RenderShader(shader);

	if (m_modelShader)
		RenderShaderRigidModels(*m_modelShader);
}


// ���� �� ���
void cTerrain::RenderRigidModels()
{
	BOOST_FOREACH (auto model, m_rigids)
	{
		model->Render();
	}
}


// ���� �� ���.
void cTerrain::RenderShaderRigidModels(cShader &shader)
{
	BOOST_FOREACH (auto model, m_rigids)
	{
		model->RenderShader(shader);
	}
}


float Lerp(float p1, float p2, float alpha)
{
	return p1 * (1.f - alpha) + p2 * alpha;
}


// x/z��鿡�� ���� ��ǥ x,z ��ġ�� �ش��ϴ� ���� �� y�� �����Ѵ�.
float cTerrain::GetHeight(const float x, const float z)
{
	const float WIDTH = m_colCellCount * m_cellSize;
	const float HEIGHT = m_rowCellCount * m_cellSize;

	float newX = x + (WIDTH / 2.0f);
	float newZ = HEIGHT - (z + (HEIGHT / 2.0f));

	newX /= m_cellSize;
	newZ /= m_cellSize;

	const float col = ::floorf( newX );
	const float row = ::floorf( newZ );

	//  A   B
	//  *---*
	//  | / |
	//  *---*
	//  C   D
	const float A = GetHeightMapEntry( (int)row, (int)col );
	const float B = GetHeightMapEntry( (int)row, (int)col+1 );
	const float C = GetHeightMapEntry( (int)row+1, (int)col );
	const float D = GetHeightMapEntry( (int)row+1, (int)col+1 );

	const float dx = newX - col;
	const float dz = newZ - row;

	float height = 0.0f;
	if( dz < 1.0f - dx )  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C
		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}


// ���� 2���� �迭�� ���� ��, row, col �ε����� ���� ���� �����Ѵ�.
float cTerrain::GetHeightMapEntry( int row, int col )
{
	const int VERTEX_COL_COUNT = m_colCellCount + 1;
	const int VERTEX_ROW_COUNT = m_rowCellCount + 1;

	const int vtxSize = (VERTEX_COL_COUNT) * (VERTEX_ROW_COUNT);

	if( 0 > row || 0 > col )
		return 0.f;
	if( vtxSize <= (row * VERTEX_ROW_COUNT + col) ) 
		return 0.f;

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();
	const float h = pv[ row * VERTEX_ROW_COUNT + col].p.y;
	m_grid.GetVertexBuffer().Unlock();
	return h;
}


// ���� ���� orig, dir �� �̿��ؼ�, �浹�� ���� y ��ǥ�� �����Ѵ�.
// ��ŷ ��ġ�� out�� �����ؼ� �����Ѵ�.
float cTerrain::GetHeightFromRay( const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out )
{
	if (m_grid.Pick(orig, dir, out))
	{
		return GetHeight(out.x, out.z);
	}
	return 0.f;
}


// ��ŷ ó��.
bool cTerrain::Pick(const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out)
{
	return m_grid.Pick(orig, dir, out);
}


// �� ��ŷ.
cModel* cTerrain::PickModel(const Vector3 &orig, const Vector3 &dir)
{
	BOOST_FOREACH (auto &model, m_rigids)
	{
		if (model->Pick(orig, dir))
			return model;
	}
	return NULL;
}


// �ʱ�ȭ.
void cTerrain::Clear()
{
	m_rowCellCount = 0;
	m_colCellCount = 0;
	m_cellSize = 0;
	m_heightFactor = 3.f;
	m_textureUVFactor = 1.f;
	m_heightMapFileName.clear();
	m_grid.Clear();

	BOOST_FOREACH (auto model, m_rigids)
	{
		SAFE_DELETE(model);
	}
	m_rigids.clear();
}


const string& cTerrain::GetTextureName()
{
	return m_grid.GetTexture().GetTextureName();
}


// ���� �� �߰�
bool cTerrain::AddRigidModel(const cModel &model)
{
	RETV(FindRigidModel(model.GetId()), false); // already exist return

	m_rigids.push_back(model.Clone());
	return true;
}


// ���� �� �߰�
cModel* cTerrain::AddRigidModel(const string &fileName)
{
	cModel *model = new cModel(common::GenerateId());
	if (!model->Create(fileName))
	{
		delete model;
		return NULL;
	}
	m_rigids.push_back(model);
	return model;
}


// ���� �� ã��.
cModel* cTerrain::FindRigidModel(const int id)
{
	BOOST_FOREACH (auto model, m_rigids)
	{
		if (model->GetId() == id)
			return model;
	}
	return NULL;
}


// ���� �� ����
// destruct : true �̸� �޸𸮸� �Ұ��Ѵ�.
bool cTerrain::RemoveRigidModel(cModel *model, const bool destruct) // destruct=true
{
	const bool result = common::removevector(m_rigids, model);
	if (destruct)
		SAFE_DELETE(model);
	return result;
}


// ���� �� ����.
bool cTerrain::RemoveRigidModel(const int id, const bool destruct) //destruct=true
{
	cModel *model = FindRigidModel(id);
	RETV(!model, false);
	return RemoveRigidModel(model, destruct);
}
