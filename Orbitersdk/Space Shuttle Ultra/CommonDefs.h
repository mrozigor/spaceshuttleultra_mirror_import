#ifndef COMMONDEFS_H
#define COMMONDEFS_H
#pragma once

#include <vector>
#include "orbiterapi.h"

const UINT MESH_UNDEFINED = (UINT)-1;

//mesh illumination functions
static void IlluminateMesh(MESHHANDLE mesh)
{
	MATERIAL* material=NULL;

	DWORD materialCount = oapiMeshMaterialCount(mesh);
    for (DWORD mi = 0; mi < materialCount; mi++) {
        material = oapiMeshMaterial(mesh, mi);
        if (material->emissive.g <= 0.1) {
            material->emissive.r = 0.3f;
            material->emissive.g = 0.3f;
            material->emissive.b = 0.3f;
        }
    }
}

static void IlluminateMesh(MESHHANDLE mesh, std::vector<int> vExclude)
{
	MATERIAL* material=NULL;
	unsigned int ExCounter=0;

	int materialCount = oapiMeshMaterialCount(mesh);
    for (int mi = 0; mi < materialCount; mi++) {
		if(vExclude[ExCounter]!=mi) {
			material = oapiMeshMaterial(mesh, mi);
			if (material->emissive.g <= 0.1) {
				material->emissive.r = 0.3f;
				material->emissive.g = 0.3f;
				material->emissive.b = 0.3f;
			}
		}
		else if(ExCounter<(vExclude.size()-1)) ExCounter++;
    }
}

static void DisableIllumination(MESHHANDLE mesh, MESHHANDLE GlobalMesh)
{
	MATERIAL* MeshMaterial=NULL;
	MATERIAL* DefaultMaterial=NULL;

	DWORD materialCount = oapiMeshMaterialCount(mesh);
    for (DWORD mi = 0; mi < materialCount; mi++) {
        MeshMaterial = oapiMeshMaterial(mesh, mi);
		DefaultMaterial = oapiMeshMaterial(GlobalMesh, mi);
        
		MeshMaterial->emissive.r=DefaultMaterial->emissive.r;
		MeshMaterial->emissive.g=DefaultMaterial->emissive.g;
		MeshMaterial->emissive.b=DefaultMaterial->emissive.b;
    }
}

#endif