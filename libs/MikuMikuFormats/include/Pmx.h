//          Copyright oguna 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

namespace pmx
{
	/// �C���f�b�N�X�ݒ�
	struct PmxSetting
	{
		/// �G���R�[�h����
		uint8_t encoding;
		/// �ǉ�UV��
		uint8_t uv;
		/// ���_�C���f�b�N�X�T�C�Y
		uint8_t vertex_index_size;
		/// �e�N�X�`���C���f�b�N�X�T�C�Y
		uint8_t texture_index_size;
		/// �}�e���A���C���f�b�N�X�T�C�Y
		uint8_t material_index_size;
		/// �{�[���C���f�b�N�X�T�C�Y
		uint8_t bone_index_size;
		/// ���[�t�C���f�b�N�X�T�C�Y
		uint8_t morph_index_size;
		/// ���̃C���f�b�N�X�T�C�Y
		uint8_t rigidbody_index_size;
		void Read(std::istream *stream);
	};

	/// ���_�X�L�j���O�^�C�v
	enum class PmxVertexSkinningType : uint8_t
	{
		BDEF1 = 0,
		BDEF2 = 1,
		BDEF4 = 2,
		SDEF = 3,
		QDEF = 4,
	};

	/// ���_�X�L�j���O
	struct PmxVertexSkinning
	{
	public:
		virtual void Read(std::istream *stream, PmxSetting *setting) = 0;
	};

	class PmxVertexSkinningBDEF1 : public PmxVertexSkinning
	{
	public:
		int bone_index;
		void Read(std::istream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningBDEF2 : public PmxVertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		float bone_weight;
		void Read(std::istream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningBDEF4 : public PmxVertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;
		void Read(std::istream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningSDEF : public PmxVertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		float bone_weight;
		float sdef_c[3];
		float sdef_r0[3];
		float sdef_r1[3];
		void Read(std::istream *stresam, PmxSetting *setting);
	};

	class PmxVertexSkinningQDEF : public PmxVertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;
		void Read(std::istream *stresam, PmxSetting *setting);
	};

	/// ���_
	struct PmxVertex
	{
		/// �ʒu
		float positon[3];
		/// �@��
		float normal[3];
		/// �e�N�X�`�����W
		float uv[2];
		/// �ǉ��e�N�X�`�����W
		float uva[4][4];
		/// �X�L�j���O�^�C�v
		PmxVertexSkinningType skinning_type;
		/// �X�L�j���O
		std::unique_ptr<PmxVertexSkinning> skinning;
		/// �G�b�W�{��
		float edge;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	/// �}�e���A��
	struct PmxMaterial
	{
		/// ���f����
		std::wstring material_name;
		/// ���f���p��
		std::wstring material_english_name;
		/// �����F
		float diffuse[4];
		/// ����F
		float specular[3];
		/// ����x
		float specularlity;
		/// ���F
		float ambient[3];
		/// �`��t���O
		uint8_t flag;
		/// �G�b�W�F
		float edge_color[4];
		/// �G�b�W�T�C�Y
		float edge_size;
		/// �A���x�h�e�N�X�`���C���f�b�N�X
		int diffuse_texture_index;
		/// �X�t�B�A�e�N�X�`���C���f�b�N�X
		int sphere_texture_index;
		/// �X�t�B�A�e�N�X�`�����Z���[�h
		uint8_t sphere_op_mode;
		/// ���L�g�D�[���t���O
		uint8_t common_toon_flag;
		/// �g�D�[���e�N�X�`���C���f�b�N�X
		int toon_texture_index;
		/// ����
		std::wstring memo;
		/// ���_�C���f�b�N�X��
		int index_count;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	/// �����N
	struct PmxIkLink
	{
		/// �����N�{�[���C���f�b�N�X
		int link_target;
		/// �p�x����
		uint8_t angle_lock;
		/// �ő吧���p�x
		float max_radian[3];
		/// �ŏ������p�x
		float min_radian[3];
		void Read(std::istream *stream, PmxSetting *settingn);
	};

	/// �{�[��
	struct PmxBone
	{
		/// �{�[����
		std::wstring bone_name;
		/// �{�[���p��
		std::wstring bone_english_name;
		/// �ʒu
		float position[3];
		/// �e�{�[���C���f�b�N�X
		int parent_index;
		/// �K�w
		int level;
		/// �{�[���t���O
		uint16_t bone_flag;
		/// ���W�I�t�Z�b�g(has Target)
		float offset[3];
		/// �ڑ���{�[���C���f�b�N�X(not has Target)
		int target_index;
		/// �t�^�e�{�[���C���f�b�N�X
		int grant_parent_index;
		/// �t�^��
		float grant_weight;
		/// �Œ莲�̕���
		float lock_axis_orientation[3];
		/// ���[�J������X������
		float local_axis_x_orientation[3];
		/// ���[�J������Y������
		float local_axis_y_orientation[3];
		/// �O���e�ό`��key�l
		int key;
		/// IK�^�[�Q�b�g�{�[��
		int ik_target_bone_index;
		/// IK���[�v��
		int ik_loop;
		/// IK���[�v�v�Z���̊p�x����(���W�A��)
		float ik_loop_angle_limit;
		/// IK�����N��
		int ik_link_count;
		/// IK�����N
		std::unique_ptr<PmxIkLink []> ik_links;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	enum class MorphType : uint8_t
	{
		Group = 0,
		Vertex = 1,
		Bone = 2,
		UV = 3,
		AdditionalUV1 = 4,
		AdditionalUV2 = 5,
		AdditionalUV3 = 6,
		AdditionalUV4 = 7,
		Matrial = 8,
		Flip = 9,
		Implus = 10,
	};

	enum class MorphCategory : uint8_t
	{
		ReservedCategory = 0,
		Eyebrow = 1,
		Eye = 2,
		Mouth = 3,
		Other = 4,
	};

	struct PmxMorphOffset
	{
		void virtual Read(std::istream *stream, PmxSetting *setting) = 0;
	};

	struct PmxMorphVertexOffset : public PmxMorphOffset
	{
		int vertex_index;
		float position_offset[3];
		void Read(std::istream *stream, PmxSetting *setting) override;
	};

	struct PmxMorphUVOffset : public PmxMorphOffset
	{
		int vertex_index;
		float uv_offset[4];
		void Read(std::istream *stream, PmxSetting *setting) override;
	};

	struct PmxMorphBoneOffset : public PmxMorphOffset
	{
		int bone_index;
		float translation[3];
		float rotation[4];
		void Read(std::istream *stream, PmxSetting *setting) override;
	};

	struct PmxMorphMaterialOffset : public PmxMorphOffset
	{
		int material_index;
		uint8_t offset_operation;
		float diffuse[4];
		float specular[3];
		float specularity;
		float ambient[3];
		float edge_color[4];
		float edge_size;
		float texture_argb[4];
		float sphere_texture_argb[4];
		float toon_texture_argb[4];
		void Read(std::istream *stream, PmxSetting *setting) override;
	};

	struct PmxMorphGroupOffset : public PmxMorphOffset
	{
		int morph_index;
		float morph_weight;
		void Read(std::istream *stream, PmxSetting *setting) override;
	};

	struct PmxMorphFlipOffset : public PmxMorphOffset
	{
		int morph_index;
		float morph_value;
		void Read(std::istream *stream, PmxSetting *setting) override;
	};

	struct PmxMorphImplusOffset : public PmxMorphOffset
	{
		int rigid_body_index;
		uint8_t is_local;
		float velocity[3];
		float angular_torque[3];
		void Read(std::istream *stream, PmxSetting *setting) override;
	};

	/// ���[�t
	struct PmxMorph
	{
		/// ���[�t��
		std::wstring morph_name;
		/// ���[�t�p��
		std::wstring morph_english_name;
		/// �J�e�S��
		MorphCategory category;
		/// ���[�t�^�C�v
		MorphType morph_type;
		/// �I�t�Z�b�g��
		int offset_count;
		/// ���_���[�t�z��
		std::unique_ptr<PmxMorphVertexOffset []> vertex_offsets;
		/// UV���[�t�z��
		std::unique_ptr<PmxMorphUVOffset []> uv_offsets;
		/// �{�[�����[�t�z��
		std::unique_ptr<PmxMorphBoneOffset []> bone_offsets;
		/// �}�e���A�����[�t�z��
		std::unique_ptr<PmxMorphMaterialOffset []> material_offsets;
		/// �O���[�v���[�t�z��
		std::unique_ptr<PmxMorphGroupOffset []> group_offsets;
		/// �t���b�v���[�t�z��
		std::unique_ptr<PmxMorphFlipOffset []> flip_offsets;
		/// �C���p���X���[�t�z��
		std::unique_ptr<PmxMorphImplusOffset []> implus_offsets;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	/// �g���v�f
	struct PmxFrameElement
	{
		/// �v�f�Ώ�
		uint8_t element_target;
		/// �v�f�ΏۃC���f�b�N�X
		int index;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	/// �\���g
	struct PmxFrame
	{
		/// �g��
		std::wstring frame_name;
		/// �g�p��
		std::wstring frame_english_name;
		/// ����g�t���O
		uint8_t frame_flag;
		/// �g���v�f��
		int element_count;
		/// �g���v�f�z��
		std::unique_ptr<PmxFrameElement []> elements;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	struct PmxRigidBody
	{
		/// ���̖�
		std::wstring girid_body_name;
		/// ���̉p��
		std::wstring girid_body_english_name;
		/// �֘A�{�[���C���f�b�N�X
		int target_bone;
		/// �O���[�v
		uint8_t group;
		/// �}�X�N
		uint16_t mask;
		/// �`��
		uint8_t shape;
		float size[3];
		float position[3];
		float orientation[3];
		float mass;
		float move_attenuation;
		float rotation_attenuation;
		float repulsion;
		float friction;
		uint8_t physics_calc_type;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	enum class PmxJointType : uint8_t
	{
		Generic6DofSpring = 0,
		Generic6Dof = 1,
		Point2Point = 2,
		ConeTwist = 3,
		Slider = 5,
		Hinge = 6
	};

	struct PmxJointParam
	{
		int rigid_body1;
		int rigid_body2;
		float position[3];
		float orientaiton[3];
		float move_limitation_min[3];
		float move_limitation_max[3];
		float rotation_limitation_min[3];
		float rotation_limitation_max[3];
		float spring_move_coefficient[3];
		float spring_rotation_coefficient[3];
		void Read(std::istream *stream, PmxSetting *setting);
	};

	struct PmxJoint
	{
		std::wstring joint_name;
		std::wstring joint_english_name;
		PmxJointType joint_type;
		PmxJointParam param;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	enum PmxSoftBodyFlag : uint8_t
	{
		BLink = 0x01,
		Cluster = 0x02,
		Link = 0x04
	};

	struct PmxAncherRigidBody
	{
		int related_rigid_body;
		int related_vertex;
		bool is_near;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	struct PmxSoftBody
	{
		std::wstring soft_body_name;
		std::wstring soft_body_english_name;
		uint8_t shape;
		int target_material;
		uint8_t group;
		uint16_t mask;
		PmxSoftBodyFlag flag;
		int blink_distance;
		int cluster_count;
		float mass;
		float collisioni_margin;
		int aero_model;
		float VCF;
		float DP;
		float DG;
		float LF;
		float PR;
		float VC;
		float DF;
		float MT;
		float CHR;
		float KHR;
		float SHR;
		float AHR;
		float SRHR_CL;
		float SKHR_CL;
		float SSHR_CL;
		float SR_SPLT_CL;
		float SK_SPLT_CL;
		float SS_SPLT_CL;
		int V_IT;
		int P_IT;
		int D_IT;
		int C_IT;
		float LST;
		float AST;
		float VST;
		int anchor_count;
		std::unique_ptr<PmxAncherRigidBody []> anchers;
		int pin_vertex_count;
		std::unique_ptr<int []> pin_vertices;
		void Read(std::istream *stream, PmxSetting *setting);
	};

	/// PMX���f��
	struct PmxModel
	{
		/// �o�[�W����
		float version;
		/// �ݒ�
		PmxSetting setting;
		/// ���f����
		std::wstring model_name;
		/// ���f���p��
		std::wstring model_english_name;
		/// �R�����g
		std::wstring model_comment;
		/// �p��R�����g
		std::wstring model_english_commnet;
		/// ���_��
		int vertex_count;
		/// ���_�z��
		std::unique_ptr<PmxVertex []> vertices;
		/// �C���f�b�N�X��
		int index_count;
		/// �C���f�b�N�X�z��
		std::unique_ptr<int []> indices;
		/// �e�N�X�`����
		int texture_count;
		/// �e�N�X�`���z��
		std::unique_ptr< std::wstring []> textures;
		/// �}�e���A����
		int material_count;
		/// �}�e���A��
		std::unique_ptr<PmxMaterial []> materials;
		/// �{�[����
		int bone_count;
		/// �{�[���z��
		std::unique_ptr<PmxBone []> bones;
		/// ���[�t��
		int morph_count;
		/// ���[�t�z��
		std::unique_ptr<PmxMorph []> morphs;
		/// �\���g��
		int frame_count;
		/// �\���g�z��
		std::unique_ptr<PmxFrame [] > frames;
		/// ���̐�
		int rigid_body_count;
		/// ���̔z��
		std::unique_ptr<PmxRigidBody []> rigid_bodies;
		/// �W���C���g��
		int joint_count;
		/// �W���C���g�z��
		std::unique_ptr<PmxJoint []> joints;
		/// �\�t�g�{�f�B��
		int soft_body_count;
		/// �\�t�g�{�f�B�z��
		std::unique_ptr<PmxSoftBody []> soft_bodies;
		/// ���f��������
		void Init();
		/// ���f���ǂݍ���
		void Read(std::istream *stream);
		///// �t�@�C�����烂�f���̓ǂݍ���
		//static std::unique_ptr<PmxModel> ReadFromFile(const char *filename);
		///// ���̓X�g���[�����烂�f���̓ǂݍ���
		//static std::unique_ptr<PmxModel> ReadFromStream(std::istream *stream);
	};
}