
#pragma once


#include <d3d9.h>
#include <d3dx9.h>

#include <objidl.h>
#include <gdiplus.h> 

#include "../Common/common.h"
using namespace common;


#include "base/graphicdef.h"
#include "base/material.h"
#include "base/light.h"
#include "base/vertexbuffer.h"
#include "base/indexbuffer.h"
#include "base/texture.h"
#include "base/vertexformat.h"
#include "base/grid.h"
#include "base/grid2.h"
#include "base/box.h"
#include "base/line.h"
#include "base/cube.h"
#include "base/shader.h"
#include "base/camera.h"
#include "base/skybox.h"
#include "base/sphere.h"
#include "model/node.h"

#include "collision/boundingbox.h"
#include "collision/collision.h"
#include "collision/collisionmanager.h"

#include "model/rawmesh.h"
#include "model/rawani.h"
#include "model/track.h"
#include "model/mesh.h"
#include "model/rigidmesh.h"
#include "model/skinnedmesh.h"
#include "model/model.h"
#include "model/bonemgr.h"
#include "model/bonenode.h"
#include "model/character.h"

#include "sprite/sprite.h"

#include "terrain/rawterrain.h"
#include "terrain/terrain.h"
#include "terrain/terraincursor.h"
#include "terrain/terraineditor.h"
#include "terrain/terrainimporter.h"
#include "terrain/terrainexporter.h"


#include "manager/Renderer.h"
#include "manager/resourcemanager.h"

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "gdiplus.lib" ) 
