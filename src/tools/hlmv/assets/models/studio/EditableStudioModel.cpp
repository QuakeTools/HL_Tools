#include <filesystem>

#include "assets/models/studio/EditableStudioModel.hpp"

namespace models::studio
{
std::vector<EditableBone> ConvertBonesToEditable(const studiohdr_t& header)
{
	const auto* const bones = header.GetData<mstudiobone_t>(header.boneindex);

	std::vector<EditableBone> list;

	list.reserve(header.numbones);

	for (decltype(header.numbones) i = 0; i < header.numbones; ++i)
	{
		const mstudiobone_t& rawBone = bones[i];

		EditableBone bone;

		bone.Name = rawBone.name;
		bone.Parent = rawBone.parent;

		for (std::size_t j = 0; j < VectorComponentCount; ++j)
		{
			bone.MovementData.BoneControllers[j] = rawBone.bonecontroller[j];
			bone.MovementData.Values[j] = rawBone.value[j];
			bone.MovementData.Scales[j] = rawBone.scale[j];

			bone.RotationData.BoneControllers[j] = rawBone.bonecontroller[VectorComponentCount + j];
			bone.RotationData.Values[j] = rawBone.value[VectorComponentCount + j];
			bone.RotationData.Scales[j] = rawBone.scale[VectorComponentCount + j];
		}

		list.emplace_back(bone);
	}

	return list;
}

std::vector<EditableBoneController> ConvertBoneControllersToEditable(const studiohdr_t& header)
{
	const auto* const boneControllers = header.GetData<mstudiobonecontroller_t>(header.bonecontrollerindex);

	std::vector<EditableBoneController> list;

	list.reserve(header.numbonecontrollers);

	for (decltype(header.numbonecontrollers) i = 0; i < header.numbonecontrollers; ++i)
	{
		const mstudiobonecontroller_t& rawBoneController = boneControllers[i];

		EditableBoneController boneController;

		boneController.Bone = rawBoneController.bone;
		boneController.Index = rawBoneController.index;
		boneController.Start = rawBoneController.start;
		boneController.End = rawBoneController.end;
		boneController.Index = rawBoneController.index;

		list.emplace_back(boneController);
	}

	return list;
}

std::vector<EditableHitbox> ConvertHitBoxesToEditable(const studiohdr_t& header)
{
	const auto* const hitboxes = header.GetData<mstudiobbox_t>(header.hitboxindex);

	std::vector<EditableHitbox> list;

	list.reserve(header.numhitboxes);

	for (decltype(header.numhitboxes) i = 0; i < header.numhitboxes; ++i)
	{
		const mstudiobbox_t& rawHitbox = hitboxes[i];

		EditableHitbox hitbox;

		hitbox.Bone = rawHitbox.bone;
		hitbox.Group = rawHitbox.group;
		hitbox.BBMins = rawHitbox.bbmin;
		hitbox.BBMaxs = rawHitbox.bbmax;

		list.emplace_back(hitbox);
	}

	return list;
}

std::vector<EditableEvent> ConvertEventsToEditable(const studiohdr_t& header, int eventIndex, int numEvents)
{
	const auto* const events = header.GetData<mstudioevent_t>(eventIndex);

	std::vector<EditableEvent> list;

	list.reserve(numEvents);

	for (decltype(numEvents) i = 0; i < numEvents; ++i)
	{
		const mstudioevent_t& rawEvent = events[i];

		EditableEvent event;

		event.Frame = rawEvent.frame;
		event.EventId = rawEvent.event;
		event.Options = rawEvent.options;

		list.emplace_back(event);
	}

	return list;
}

std::vector<std::vector<EditableAnimation>> ConvertAnimationBlendsToEditable(
	const std::uint8_t* const buffer, int numBones, const mstudioseqdesc_t& rawSequence)
{
	const auto* const animationBase = reinterpret_cast<const mstudioanim_t*>(buffer + rawSequence.animindex);

	std::vector<std::vector<EditableAnimation>> list{};

	list.reserve(rawSequence.numblends);

	//Each blend has an animation for each bone
	//Each animation has values for translation and rotation
	for (int blend = 0; blend < rawSequence.numblends; ++blend)
	{
		const auto* const animationBlend = animationBase + (numBones * blend);

		//Read each animation
		std::vector<EditableAnimation> animations;

		animations.reserve(numBones);

		for (int bone = 0; bone < numBones; ++bone)
		{
			const auto& rawAnimation = animationBlend[bone];

			EditableAnimation animation;

			for (int component = 0; component < VectorComponentCount; ++component)
			{
				//Leave the list null if there is no offset
				if (rawAnimation.offset[component] != 0)
				{
					const auto* value = reinterpret_cast<const mstudioanimvalue_t*>(
						reinterpret_cast<const std::uint8_t*>(animationBlend) + rawAnimation.offset[component] + (sizeof(mstudioanim_t) * bone));

					std::vector<short> values;

					values.reserve(rawSequence.numframes);

					for (int frame = 0; frame < rawSequence.numframes; )
					{
						const auto count = *value++;

						int i;

						//Add all valid values
						for (i = 0; i < count.num.valid; ++i)
						{
							const auto animValue = *value++;

							values.emplace_back(animValue.value);
						}

						//If it's a run, convert it
						if (i < count.num.total)
						{
							const auto lastValue = values[values.size() - 1];

							for (; i < count.num.total; ++i)
							{
								values.emplace_back(lastValue);
							}
						}

						frame += count.num.total;
					}

					animation.Values[component] = std::move(values);
				}
			}

			animations.emplace_back(std::move(animation));
		}

		list.emplace_back(std::move(animations));
	}

	return list;
}

std::vector<EditablePivot> ConvertPivotsToEditable(const studiohdr_t& header, const mstudioseqdesc_t& rawSequence)
{
	const auto* const pivots = header.GetData<mstudiopivot_t>(rawSequence.pivotindex);

	std::vector<EditablePivot> list;

	list.reserve(rawSequence.numpivots);

	for (decltype(rawSequence.numpivots) i = 0; i < rawSequence.numpivots; ++i)
	{
		const mstudiopivot_t& rawPivot = pivots[i];

		EditablePivot pivot;

		pivot.Origin = rawPivot.org;
		pivot.Start = rawPivot.start;
		pivot.End = rawPivot.end;

		list.emplace_back(pivot);
	}

	return list;
}

std::vector<EditableSequence> ConvertSequencesToEditable(const studiohdr_t& header, const StudioData& data)
{
	const auto* const sequences = header.GetData<mstudioseqdesc_t>(header.seqindex);

	std::vector<EditableSequence> list;

	list.reserve(header.numseq);

	for (decltype(header.numseq) i = 0; i < header.numseq; ++i)
	{
		const mstudioseqdesc_t& rawSequence = sequences[i];

		EditableSequence sequence;

		sequence.Label = rawSequence.label;

		sequence.FPS = rawSequence.fps;

		sequence.Flags = rawSequence.flags;

		sequence.Activity = rawSequence.activity;
		sequence.ActivityWeight = rawSequence.actweight;

		sequence.Events = ConvertEventsToEditable(header, rawSequence.eventindex, rawSequence.numevents);

		sequence.Pivots = ConvertPivotsToEditable(header, rawSequence);

		sequence.MotionType = rawSequence.motiontype;
		sequence.MotionBone = rawSequence.motionbone;

		sequence.LinearMovement = rawSequence.linearmovement;

		sequence.BBMin = rawSequence.bbmin;
		sequence.BBMax = rawSequence.bbmax;

		sequence.EntryNode = rawSequence.entrynode;
		sequence.ExitNode = rawSequence.exitnode;
		sequence.NodeFlags = rawSequence.nodeflags;

		sequence.FrameCount = rawSequence.numframes;
		sequence.SequenceGroup = rawSequence.seqgroup;

		for (std::size_t blend = 0; blend < BlendDataCount; ++blend)
		{
			auto& blendType = sequence.BlendData[blend];

			blendType.Type = rawSequence.blendtype[blend];
			blendType.Start = rawSequence.blendstart[blend];
			blendType.End = rawSequence.blendend[blend];
		}

		//Determine which stream to read from
		//TODO: validate seqgroup value to prevent invalid access
		const std::uint8_t* animationBuffer = nullptr;

		if (rawSequence.seqgroup == 0)
		{
			animationBuffer = reinterpret_cast<const std::uint8_t*>(&header);
		}
		else
		{
			const auto& group = data.SequenceGroupData[rawSequence.seqgroup - 1];
			animationBuffer = reinterpret_cast<const std::uint8_t*>(group.get());
		}

		//Can be null if a sequence file failed to load
		if (animationBuffer)
		{
			sequence.Blends = ConvertAnimationBlendsToEditable(animationBuffer, header.numbones, rawSequence);
		}

		list.emplace_back(std::move(sequence));
	}

	return list;
}

static std::vector<EditableSequenceGroup> ConvertSequenceGroupsToEditable(const studiohdr_t& header)
{
	const auto* const sequenceGroups = header.GetData<mstudioseqgroup_t>(header.seqgroupindex);

	std::vector<EditableSequenceGroup> list;

	list.reserve(header.numseqgroups);

	for (int i = 0; i < header.numseqgroups; ++i)
	{
		EditableSequenceGroup group;

		group.Label = sequenceGroups[i].label;
		group.FileName = sequenceGroups[i].name;

		list.emplace_back(std::move(group));
	}

	return list;
}

std::vector<EditableBodypart> ConvertBodyPartsToEditable(const studiohdr_t& header)
{
	const auto* const bodyParts = header.GetData<mstudiobodyparts_t>(header.bodypartindex);

	std::vector<EditableBodypart> list;

	list.reserve(header.numbodyparts);

	for (int i = 0; i < header.numbodyparts; ++i)
	{
		const mstudiobodyparts_t& rawBodyPart = bodyParts[i];

		std::vector<EditableModel> models;

		models.reserve(rawBodyPart.nummodels);

		const auto* const rawModels = header.GetData<mstudiomodel_t>(rawBodyPart.modelindex);

		for (int model = 0; model < rawBodyPart.nummodels; ++model)
		{
			const mstudiomodel_t& rawBodyModel = rawModels[model];

			const auto* const rawVertexBones = header.GetData<std::uint8_t>(rawBodyModel.vertinfoindex);
			const auto* const rawVertices = header.GetData<glm::vec3>(rawBodyModel.vertindex);
			const auto* const rawNormalBones = header.GetData<std::uint8_t>(rawBodyModel.norminfoindex);
			const auto* const rawNormals = header.GetData<glm::vec3>(rawBodyModel.normindex);

			std::vector<EditableMesh> meshes;

			meshes.reserve(rawBodyModel.nummesh);

			const auto* const rawMeshes = header.GetData<mstudiomesh_t>(rawBodyModel.meshindex);

			for (int mesh = 0; mesh < rawBodyModel.nummesh; ++mesh)
			{
				const mstudiomesh_t& rawMesh = rawMeshes[mesh];

				std::vector<EditableMeshTriangleRun> triangles;

				//numtris is the upper limit for the triangles list, assuming all runs are only a single triangle
				triangles.reserve(rawMesh.numtris);

				const auto* rawCommands = header.GetData<short>(rawMesh.triindex);

				for (short command = *rawCommands++;
					command != 0;
					command = *rawCommands++)
				{
					std::vector<EditableMeshVertex> vertices;

					const auto numCommands = std::abs(command);

					vertices.reserve(numCommands);

					for (auto commandsLeft = numCommands; commandsLeft > 0; --commandsLeft, rawCommands += 4)
					{
						const auto vertexIndex = rawCommands[0];
						const auto normalIndex = rawCommands[1];

						EditableMeshVertex vertex;

						vertex.VertexBone = rawVertexBones[vertexIndex];
						vertex.Vertex = rawVertices[vertexIndex];
						vertex.NormalBone = rawNormalBones[normalIndex];
						vertex.Normal = rawNormals[normalIndex];
						vertex.S = rawCommands[2];
						vertex.T = rawCommands[3];
					}

					EditableMeshTriangleRun triangleRun;

					triangleRun.Vertices = std::move(vertices);
					triangleRun.IsStrip = command > 0;

					triangles.emplace_back(std::move(triangleRun));
				}

				triangles.shrink_to_fit();

				EditableMesh bodyMesh;

				bodyMesh.Triangles = std::move(triangles);
				bodyMesh.Texture = rawMesh.skinref;

				meshes.emplace_back(std::move(bodyMesh));
			}

			EditableModel bodyModel;

			bodyModel.Name = rawBodyModel.name;
			bodyModel.Meshes = std::move(meshes);

			models.emplace_back(std::move(bodyModel));
		}

		EditableBodypart part;

		part.Name = rawBodyPart.name;
		part.Base = rawBodyPart.base;
		part.Models = std::move(models);

		list.emplace_back(part);
	}

	return list;
}

std::vector<EditableAttachment> ConvertAttachmentsToEditable(const studiohdr_t& header)
{
	const auto* const rawAttachments = header.GetData<mstudioattachment_t>(header.attachmentindex);

	std::vector<EditableAttachment> list;

	list.reserve(header.numattachments);

	for (int i = 0; i < header.numattachments; ++i)
	{
		const auto& rawAttachment = rawAttachments[i];

		EditableAttachment attachment;

		attachment.Name = rawAttachment.name;
		attachment.Bone = rawAttachment.bone;
		attachment.Origin = rawAttachment.org;

		list.emplace_back(std::move(attachment));
	}

	return list;
}

std::vector<std::vector<std::uint8_t>> ConvertTransitionsToEditable(const studiohdr_t& header)
{
	const auto* const rawTransitions = header.GetData<std::uint8_t>(header.transitionindex);

	std::vector<std::vector<std::uint8_t>> list;

	list.reserve(header.numtransitions);

	for (int i = 0; i < header.numtransitions; ++i)
	{
		std::vector<std::uint8_t> mappings(rawTransitions + (i * header.numtransitions), rawTransitions + ((i + 1) * header.numtransitions));

		list.emplace_back(std::move(mappings));
	}

	return list;
}

EditableTexture ConvertMdlTextureToEditable(const studiohdr_t& header, const mstudiotexture_t& texture)
{
	const std::uint8_t* pTextureData = header.GetData<std::uint8_t>(texture.index);

	std::vector<std::uint8_t> pixels(texture.width * texture.height);

	memcpy(pixels.data(), pTextureData, pixels.size());

	pTextureData += pixels.size();

	RGB24Palette palette;

	memcpy(&palette, pTextureData, sizeof(palette));

	pTextureData += sizeof(palette);

	return EditableTexture
	{
		texture.name,
		texture.flags,
		RGB24Image
		{
			palette,
			std::move(pixels),
			static_cast<unsigned int>(texture.width),
			static_cast<unsigned int>(texture.height)
		}
	};
}

//Dol differs only in texture storage when it comes to existing data structures
//Instead of pixels followed by RGB palette, it has a 32 byte texture name (name of file without extension), followed by an RGBA palette and pixels
EditableTexture ConvertDolTextureToEditable(const studiohdr_t& header, const mstudiotexture_t& texture)
{
	//Starts off with 32 byte texture name
	const std::uint8_t* pTextureData = header.GetData<std::uint8_t>(texture.index + 32);

	RGBA32Palette sourcePalette;

	memcpy(&sourcePalette, pTextureData, sizeof(sourcePalette));

	pTextureData += sizeof(sourcePalette);

	RGB24Palette palette;

	//Discard alpha value
	//TODO: convert alpha value somehow? is it even used?
	for (decltype(sourcePalette.size()) i = 0; i < sourcePalette.size(); ++i)
	{
		palette[i] = sourcePalette[i].AsRGB();
	}

	const auto size = texture.width * texture.height;

	std::vector<std::uint8_t> pixels(size);

	for (int i = 0; i < size; ++i)
	{
		auto pixel = *pTextureData++;

		auto masked = pixel & 0x1F;

		//Adjust the index to map to the correct palette entry
		if (masked >= 8)
		{
			if (masked >= 16)
			{
				if (masked < 24)
				{
					pixel -= 8;
				}
			}
			else
			{
				pixel += 8;
			}
		}

		pixels[i] = pixel;
	}

	return EditableTexture
	{
		texture.name,
		texture.flags,
		RGB24Image
		{
			palette,
			std::move(pixels),
			static_cast<unsigned int>(texture.width),
			static_cast<unsigned int>(texture.height)
		}
	};
}

std::vector<EditableTexture> ConvertTexturesToEditable(const studiohdr_t& header, const bool isDol)
{
	const auto* const rawTextures = header.GetData<mstudiotexture_t>(header.textureindex);

	std::vector<EditableTexture> list;

	list.reserve(header.numtextures);

	for (int i = 0; i < header.numtextures; ++i)
	{
		const mstudiotexture_t& rawTexture = rawTextures[i];

		if (isDol)
		{
			list.emplace_back(ConvertDolTextureToEditable(header, rawTexture));
		}
		else
		{
			list.emplace_back(ConvertMdlTextureToEditable(header, rawTexture));
		}
	}

	return list;
}

std::vector<std::vector<short>> ConvertSkinsToEditable(const studiohdr_t& header)
{
	const auto* const rawSkinFamilies = header.GetData<short>(header.skinindex);

	std::vector<std::vector<short>> list;

	list.reserve(header.numskinfamilies);

	for (int i = 0; i < header.numskinfamilies; ++i)
	{
		std::vector<short> skinRefs(rawSkinFamilies + (i * header.numskinref), rawSkinFamilies + ((i + 1) * header.numskinref));

		list.emplace_back(std::move(skinRefs));
	}

	return list;
}

EditableStudioModel ConvertToEditable(const std::filesystem::path& fileName, const StudioData& data)
{
	const auto& header = *data.MainData;

	EditableStudioModel model{};

	model.EyePosition = header.eyeposition;
	model.Mins = header.min;
	model.Maxs = header.max;
	model.BBMins = header.bbmin;
	model.BBMaxs = header.bbmax;
	model.Flags = header.flags;

	model.Bones = ConvertBonesToEditable(header);
	model.BoneControllers = ConvertBoneControllersToEditable(header);
	model.Hitboxes = ConvertHitBoxesToEditable(header);
	model.Sequences = ConvertSequencesToEditable(header, data);
	model.SequenceGroups = ConvertSequenceGroupsToEditable(header);
	model.Bodyparts = ConvertBodyPartsToEditable(header);
	model.Attachments = ConvertAttachmentsToEditable(header);
	model.Transitions = ConvertTransitionsToEditable(header);

	const auto isDol = fileName.extension() == ".dol";

	auto readTextureData = [&](const studiohdr_t& textureHeader)
	{
		model.Textures = ConvertTexturesToEditable(textureHeader, isDol);
		model.SkinFamilies = ConvertSkinsToEditable(textureHeader);
	};

	//The texture file or the main file contains this data
	if (header.textureindex != 0)
	{
		readTextureData(header);
	}
	else if (const auto& textureHeader = data.TextureData; textureHeader)
	{
		readTextureData(*textureHeader);
	}

	return model;
}

/*
StudioData ConvertFromEditable(const EditableStudioModel& model)
{

}
*/
}
