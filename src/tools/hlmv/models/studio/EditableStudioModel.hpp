#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <glm/vec3.hpp>

#include "models/Model.hpp"

#include "models/studio/StudioFileFormat.hpp"
#include "models/studio/StudioIO.hpp"

#include "utility/Image.hpp"

namespace models::studio
{
struct Bone3DData
{
	std::array<int, VectorComponentCount> BoneControllers{NoBoneController, NoBoneController, NoBoneController};

	std::array<float, VectorComponentCount> Values;

	std::array<float, VectorComponentCount> Scales;
};

/**
*	@details Bone flags are not included because they aren't used
*/
struct EditableBone
{
	std::string Name;
	int Parent = NoBoneParent;

	Bone3DData MovementData;
	Bone3DData RotationData;
};

/**
*	@details Rest value is not included because it isn't used
*/
struct EditableBoneController
{
	//It is an error to have a controller with no bone
	int Bone = NoBone;

	int Type = 0;

	float Start = 0;
	float End = 0;

	//It is an error to have a controller with no index
	int Index = NoBoneControllerIndex;
};

struct EditableHitbox
{
	int Bone = NoBone;
	int Group = NoHitboxGroup;

	glm::vec3 BBMins;
	glm::vec3 BBMaxs;
};

/**
*	@details unused1 and unused2 are not included because they were deprecated
*/
struct EditableSequenceGroup
{
	std::string Label;
	std::string FileName;
};

/**
*	@details type is not included because it is not used
*/
struct EditableEvent
{
	int Frame = 0;
	int EventId = 0;

	std::string Options;
};

struct EditablePivot
{
	glm::vec3 Origin;
	int Start = 0;
	int End = 0;
};

struct EditableBlend
{
	int Type = 0;
	float Start = 0;
	float End = 0;
};

/**
*	@brief Represents a single frame of animation for a single animation blend
*/
struct EditableAnimation
{
	std::array<std::vector<short>, TotalAnimationComponentCount> Values;
};

/**
*	@details automoveposindex, automoveangleindex, blendparent, nextseq are not included because they are not used
*/
struct EditableSequence
{
	std::string Label;

	float FPS = 0;
	int Flags = 0;

	int Activity = 0;
	int ActivityWeight = 0;

	std::vector<EditableEvent> Events;

	std::vector<EditablePivot> Pivots;

	int FrameCount = 0;

	int MotionType = 0;
	int MotionBone = NoBone;

	glm::vec3 LinearMovement;

	glm::vec3 BBMin;
	glm::vec3 BBMax;

	std::vector<std::vector<EditableAnimation>> Blends;

	std::array<EditableBlend, BlendDataCount> BlendData;

	int SequenceGroup = 0;

	int EntryNode = 0;
	int ExitNode = 0;
	int NodeFlags = 0;
};

/**
*	@details type, vectors are not included because they are not used
*/
struct EditableAttachment
{
	std::string Name;
	//It is an error to have an attachment with no bone
	int Bone = NoBone;
	glm::vec3 Origin;
};

struct EditableMeshVertex
{
	int VertexBone = NoBone;
	glm::vec3 Vertex;

	int NormalBone = NoBone;
	glm::vec3 Normal;

	short S = 0;
	short T = 0;
};

//TODO: convert to non-run triangles for easier editing?
struct EditableMeshTriangleRun
{
	std::vector<EditableMeshVertex> Vertices;

	//true == strip
	//false == fan
	bool IsStrip = false;
};

struct EditableMesh
{
	std::vector<EditableMeshTriangleRun> Triangles;

	int Texture = NoTexture;
};

/**
*	@details boundingradius, type, numgroups, groupindex are not included because they are not used
*/
struct EditableModel
{
	std::string Name;

	std::vector<EditableMesh> Meshes;
};

struct EditableBodypart
{
	std::string Name;

	int Base = 0;

	std::vector<EditableModel> Models;
};

struct EditableTexture
{
	std::string Name;

	int Flags = 0;

	RGB24Image Image;
};

class EditableStudioModel : public Model
{
public:
	EditableStudioModel() = default;

	EditableStudioModel(const EditableStudioModel&) = delete;
	EditableStudioModel& operator=(const EditableStudioModel&) = delete;

	EditableStudioModel(EditableStudioModel&&) = default;
	EditableStudioModel& operator=(EditableStudioModel&&) = default;

	~EditableStudioModel() = default;

	glm::vec3 EyePosition;

	glm::vec3 Mins;
	glm::vec3 Maxs;

	glm::vec3 BBMins;
	glm::vec3 BBMaxs;

	int Flags = 0;

	std::vector<EditableBone> Bones;
	std::vector<EditableBoneController> BoneControllers;
	std::vector<EditableHitbox> Hitboxes;
	std::vector<EditableSequenceGroup> SequenceGroups;
	std::vector<EditableSequence> Sequences;
	std::vector<EditableAttachment> Attachments;
	std::vector<EditableBodypart> Bodyparts;
	std::vector<EditableTexture> Textures;
	std::vector<std::vector<short>> SkinFamilies;
	std::vector<std::vector<std::uint8_t>> Transitions;
};

EditableStudioModel ConvertToEditable(const std::string& fileName, const StudioData& data);

//TODO: implement
StudioData ConvertFromEditable(const EditableStudioModel& model);
}
