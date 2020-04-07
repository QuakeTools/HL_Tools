/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <glm/vec3.hpp>

#include "utility/math/MathDefinitions.hpp"

/**
*	@file
*
*	STUDIO MODELS
*	Studio models are position independent, so the cache manager can move them.
*/
 
namespace models::studio
{
namespace limits
{
constexpr std::size_t MaxTriangles		= 20000;	// TODO: tune this
constexpr std::size_t MaxVerts			= 2048;		// TODO: tune this
constexpr std::size_t MaxSequences		= 2048;		//! total animation sequences -- KSH incremented
constexpr std::size_t MaxSkins			= 100;		//! total textures
constexpr std::size_t MaxSourceBones	= 512;		//! bones allowed at source movement
constexpr std::size_t MaxBones			= 128;		//! total bones actually used
constexpr std::size_t MaxModels			= 32;		//! sub-models per model
constexpr std::size_t MaxBodyparts		= 32;
constexpr std::size_t MaxGroups			= 16;
constexpr std::size_t MaxAnimations		= 2048;
constexpr std::size_t MaxMeshes			= 256;
constexpr std::size_t MaxEvents			= 1024;
constexpr std::size_t MaxPivots			= 256;
constexpr std::size_t MaxControllers	= 8;
}

constexpr int NoBone = -1;
constexpr int NoBoneParent = -1;
constexpr int NoBoneController = -1;
constexpr int NoBoneControllerIndex = -1;
constexpr int NoHitboxGroup = -1;
constexpr int NoTexture = -1;

//Everywhere you see this constant, you know the code is operating on data that concerns 3D data
constexpr std::size_t VectorComponentCount = 3;

//Motion + rotation
constexpr std::size_t TotalAnimationComponentCount = VectorComponentCount * 2;

//Sequences store blend data for 2 blends, but allows more than 2 blend animations
constexpr std::size_t BlendDataCount = 2;

// lighting options
constexpr int TextureFlagFlatShade	= 0x0001;
constexpr int TextureFlagChrome		= 0x0002;
constexpr int TextureFlagFullbright	= 0x0004;
constexpr int TextureFlagNoMips		= 0x0008;
constexpr int TextureFlagAlpha		= 0x0010;
constexpr int TextureFlagAdditive	= 0x0020;
constexpr int TextureFlagMasked		= 0x0040;

// motion flags
constexpr int MotionFlagX		= 0x0001;
constexpr int MotionFlagY		= 0x0002;
constexpr int MotionFlagZ		= 0x0004;
constexpr int MotionFlagXR		= 0x0008;
constexpr int MotionFlagYR		= 0x0010;
constexpr int MotionFlagZR		= 0x0020;
constexpr int MotionFlagLX		= 0x0040;
constexpr int MotionFlagLY		= 0x0080;
constexpr int MotionFlagLZ		= 0x0100;
constexpr int MotionFlagAX		= 0x0200;
constexpr int MotionFlagAY		= 0x0400;
constexpr int MotionFlagAZ		= 0x0800;
constexpr int MotionFlagAXR		= 0x1000;
constexpr int MotionFlagAYR		= 0x2000;
constexpr int MotionFlagAZR		= 0x4000;
constexpr int MotionFlagTypes	= 0x7FFF;
constexpr int MotionFlagRLoop	= 0x8000;	//! controller that wraps shortest distance

// sequence flags
constexpr int SequenceFlagLooping = 0x0001;

constexpr int SequenceNodeFlagReverse = 1 << 0;

template<typename T>
inline constexpr T RadToStudio{32768.0 / math::PI<T>};

template<typename T>
inline constexpr T StudioToRad{math::PI<T> / 32768.0};

constexpr int FileVersion = 10;

/**
*	@brief Id for studio headers (main and texture headers)
*/
inline constexpr std::string_view MainHeaderId{"IDST"};

/**
*	@brief Id for studio sequence groups
*/
inline constexpr std::string_view SequenceGroupHeaderId{"IDSQ"};

struct studiohdr_t
{
	int					id;
	int					version;

	char				name[64];
	int					length;

	glm::vec3			eyeposition;	//! ideal eye position
	glm::vec3			min;			//! ideal movement hull size
	glm::vec3			max;			

	glm::vec3			bbmin;			//! clipping bounding box
	glm::vec3			bbmax;		

	int					flags;

	int					numbones;			//! bones
	int					boneindex;

	int					numbonecontrollers;		//! bone controllers
	int					bonecontrollerindex;

	int					numhitboxes;			//! complex bounding boxes
	int					hitboxindex;			
	
	int					numseq;				//! animation sequences
	int					seqindex;

	int					numseqgroups;		//! demand loaded sequences
	int					seqgroupindex;

	int					numtextures;		//! raw textures
	int					textureindex;
	int					texturedataindex;

	int					numskinref;			//! replaceable textures
	int					numskinfamilies;
	int					skinindex;

	int					numbodyparts;		
	int					bodypartindex;

	int					numattachments;		//! queryable attachable points
	int					attachmentindex;

	int					soundtable;
	int					soundindex;
	int					soundgroups;
	int					soundgroupindex;

	int					numtransitions;		//! animation node to animation node transition graph
	int					transitionindex;

	const std::uint8_t* GetBaseAddress() const { return reinterpret_cast<const std::uint8_t*>(this); }
	std::uint8_t* GetBaseAddress() { return reinterpret_cast<std::uint8_t*>(this); }

	template<typename T>
	const T* GetData(int offset) const
	{
		return reinterpret_cast<const T*>(GetBaseAddress() + offset);
	}

	template<typename T>
	T* GetData(int offset)
	{
		return reinterpret_cast<T*>(GetBaseAddress() + offset);
	}
};

// header for demand loaded sequence group data
struct studioseqhdr_t
{
	int					id;
	int					version;

	char				name[64];
	int					length;
};

// bones
struct mstudiobone_t
{
	char				name[32];	// bone name for symbolic links
	int		 			parent;		// parent bone
	int					flags;		// ??
	int					bonecontroller[6];	// bone controller index, -1 == none
	float				value[6];	// default DoF values
	float				scale[6];   // scale for delta DoF values
};


// bone controllers
struct mstudiobonecontroller_t
{
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					index;	// 0-3 user set controller, 4 mouth
};

// intersection boxes
struct mstudiobbox_t
{
	int					bone;
	int					group;			// intersection group
	glm::vec3			bbmin;		// bounding box
	glm::vec3			bbmax;		
};

//
// demand loaded sequence groups
//
struct mstudioseqgroup_t
{
	char				label[32];	// textual name
	char				name[64];	// file name
	std::int32_t		unused1;    // was "cache"  - index pointer
	int					unused2;    // was "data" -  hack for group 0
};

// sequence descriptions
struct mstudioseqdesc_t
{
	char				label[32];	// sequence label

	float				fps;		// frames per second	
	int					flags;		// looping/non-looping flags

	int					activity;
	int					actweight;

	int					numevents;
	int					eventindex;

	int					numframes;	// number of frames per sequence

	int					numpivots;	// number of foot pivots
	int					pivotindex;

	int					motiontype;	
	int					motionbone;
	glm::vec3			linearmovement;
	int					automoveposindex;
	int					automoveangleindex;

	glm::vec3			bbmin;		// per sequence bounding box
	glm::vec3			bbmax;		

	int					numblends;
	int					animindex;		// mstudioanim_t pointer relative to start of sequence group data
										// [blend][bone][X, Y, Z, XR, YR, ZR]

	int					blendtype[2];	// X, Y, Z, XR, YR, ZR
	float				blendstart[2];	// starting value
	float				blendend[2];	// ending value
	int					blendparent;

	int					seqgroup;		// sequence group for demand loading

	int					entrynode;		// transition node at entry
	int					exitnode;		// transition node at exit
	int					nodeflags;		// transition rules
	
	int					nextseq;		// auto advancing sequences
};

// events

struct mstudioevent_t
{
	int 				frame;
	int					event;
	int					type;
	char				options[64];
};

// pivots
struct mstudiopivot_t
{
	glm::vec3			org;	// pivot point
	int					start;
	int					end;
};

// attachment
struct mstudioattachment_t
{
	char				name[32];
	int					type;
	int					bone;
	glm::vec3			org;	// attachment point
	glm::vec3			vectors[3];
};

struct mstudioanim_t
{
	unsigned short	offset[6];
};

// animation frames
union mstudioanimvalue_t
{
	struct
	{
		std::uint8_t valid;
		std::uint8_t total;
	} num;
	short		value;
};



// body part index
struct mstudiobodyparts_t
{
	char				name[64];
	int					nummodels;
	int					base;
	int					modelindex; // index into models array
};



// skin info
struct mstudiotexture_t
{
	char					name[64];
	int						flags;
	int						width;
	int						height;
	int						index;
};


// skin families
// short	index[skinfamilies][skinref]

// studio models
struct mstudiomodel_t
{
	char				name[64];

	int					type;

	float				boundingradius;

	int					nummesh;
	int					meshindex;

	int					numverts;		// number of unique vertices
	int					vertinfoindex;	// vertex bone info
	int					vertindex;		// vertex vec3_t
	int					numnorms;		// number of unique surface normals
	int					norminfoindex;	// normal bone info
	int					normindex;		// normal vec3_t

	int					numgroups;		// deformation groups
	int					groupindex;
};


// vec3_t	boundingbox[model][bone][2];	// complex intersection info


// meshes
struct mstudiomesh_t
{
	int					numtris;
	int					triindex;
	int					skinref;
	int					numnorms;		// per mesh normals
	int					normindex;		// normal vec3_t
};

// triangles
#if 0
struct mstudiotrivert_t
{
	short				vertindex;		// index into vertex array
	short				normindex;		// index into normal array
	short				s,t;			// s,t position on skin
};
#endif


}
