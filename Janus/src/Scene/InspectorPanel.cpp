#include "jnpch.h"
#include "InspectorPanel.h"
#include <imgui.h>
#include "imgui_internal.h"
#include "Core/Application.h"
#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGui/ImGui.h"
namespace Janus {

    static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		bool modified = false;

		UI::PushID();
		ImGui::TableSetColumnIndex(0);
		UI::ShiftCursor(17.0f, 7.0f);

		ImGui::Text(label.c_str());
		UI::Draw::Underline(false, 0.0f, 2.0f);

		ImGui::TableSetColumnIndex(1);
		UI::ShiftCursor(7.0f, 0.0f);

		{
			const float spacingX = 8.0f;
			UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });
			
			{
				// Begin XYZ area
				UI::ScopedColour padding(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
				UI::ScopedStyle frame(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));

				ImGui::BeginChild(ImGui::GetID((label + "fr").c_str()),
								  ImVec2(ImGui::GetContentRegionAvail().x - spacingX, ImGui::GetFrameHeightWithSpacing() + 8.0f),
								  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			}
			const float framePadding = 2.0f;
			const float outlineSpacing = 1.0f;
			const float lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
			const ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
			const float inputItemWidth = (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;

			const ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			auto drawControl = [&] (const std::string& label, float& value, const ImVec4& colourN,
																			const ImVec4& colourH,
																			const ImVec4& colourP)
			{
				{
					UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
					UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
					UI::ScopedColourStack buttonColours(ImGuiCol_Button, colourN,
														ImGuiCol_ButtonHovered, colourH,
														ImGuiCol_ButtonActive, colourP);

					UI::ScopedFont buttonFont(boldFont);

					UI::ShiftCursorY(2.0f);
					if (ImGui::Button(label.c_str(), buttonSize))
					{
						value = resetValue;
						modified = true;
					}
				}

				ImGui::SameLine(0.0f, outlineSpacing);
				ImGui::SetNextItemWidth(inputItemWidth);
				UI::ShiftCursorY(-2.0f);
				modified |= ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");

				if (!UI::IsItemDisabled())
					UI::DrawItemActivityOutline(2.0f, true, Colours::Theme::accent);
			};


			drawControl("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("Z", values.z, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

			ImGui::EndChild();
		}
		UI::PopID();

		return modified;
	}

   	InspectorPanel::InspectorPanel(const Ref<Scene>& context)
		: m_Context(context)
	{
    
	}
	void InspectorPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}

    void InspectorPanel::SetSelected(Entity entity)
	{
		m_SelectionContext = entity;

		//if (m_SelectionChangedCallback)
			//m_SelectionChangedCallback(m_SelectionContext);
	}

	void InspectorPanel::OnImGuiRender(bool window)
	{
        if (window)
		    ImGui::Begin("Inspector");
        
        
        if (m_SelectionContext)
        {

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			auto id = m_SelectionContext.GetComponent<IDComponent>().ID;
			ImGui::SameLine();
			ImGui::TextDisabled("%llx", id);

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 textSize = ImGui::CalcTextSize("Add Component");
			textSize.x += GImGui->Style.FramePadding.x * 2.0f;
			
			ImGui::SameLine(contentRegionAvailable.x - (textSize.x + GImGui->Style.FramePadding.y));
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponentPanel");
			
			{
				if (UI::BeginPopup("AddComponentPanel"))
				{
					if(!m_SelectionContext.HasComponent<SkyLightComponent>())
					{
						if (ImGui::MenuItem("Sky Light"))
						{
							m_SelectionContext.AddComponent<SkyLightComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					if (!m_SelectionContext.HasComponent<MeshComponent>())
					{
						if (ImGui::MenuItem("Mesh"))
						{
							m_SelectionContext.AddComponent<MeshComponent>();
							ImGui::CloseCurrentPopup();
						}
					}

					if (!m_SelectionContext.HasComponent<TransformComponent>())
					{
						if (ImGui::MenuItem("Transform"))
						{
							m_SelectionContext.AddComponent<TransformComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					if (!m_SelectionContext.HasComponent<PointLightComponent>())
					{
						if (ImGui::MenuItem("Point Light"))
						{
							m_SelectionContext.AddComponent<PointLightComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
					UI::EndPopup();
				}	
			}
			if (m_SelectionContext.HasComponent<Janus::TagComponent>())
            {
                DrawTagComponent(m_SelectionContext.GetComponent<Janus::TagComponent>());
            }
			if (m_SelectionContext.HasComponent<Janus::TransformComponent>())
            {
                DrawTransformationComponent(m_SelectionContext.GetComponent<Janus::TransformComponent>());
            }
            if (m_SelectionContext.HasComponent<Janus::MeshComponent>())
            {
                Janus::Ref<Janus::Mesh> meshComponent = m_SelectionContext.GetComponent<MeshComponent>();
                if(meshComponent) {
                    DrawMeshComponent(meshComponent);
                }
            }
			if (m_SelectionContext.HasComponent<Janus::SkyLightComponent>())
            {
            	DrawSkylightComponent(m_SelectionContext.GetComponent<SkyLightComponent>());
            }

			if (m_SelectionContext.HasComponent<Janus::PointLightComponent>())
            {
            	DrawPointLightComponent(m_SelectionContext.GetComponent<PointLightComponent>());
            }
        }
        
        if (window)
			ImGui::End();
    }

	void InspectorPanel::DrawTagComponent(TagComponent& tagComponent) {
		if (ImGui::CollapsingHeader("Tag", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
			std::string& name = (std::string&)tagComponent; 
			UI::BeginPropertyGrid();
			UI::Property("Name", name);
			UI::EndPropertyGrid();	
		}
	}

	void InspectorPanel::DrawPointLightComponent(PointLightComponent& pointLightComponent) {
		if (ImGui::CollapsingHeader("Point Light", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
			UI::BeginPropertyGrid();
			UI::PropertyColor("Radiance", pointLightComponent.Radiance);
			UI::Property("Intensity", pointLightComponent.Intensity, 0.05f, 0.f, 500.f);
			//UI::Property("Source Size", dlc.LightSize, 0.05f, 0.f, std::numeric_limits<float>::max());
			//UI::Property("Min Radius", dlc.MinRadius, 0.05f, 0.f, std::numeric_limits<float>::max());
			UI::Property("Radius", pointLightComponent.Radius, 0.1f, 0.f, std::numeric_limits<float>::max());
			//UI::Property("Cast Shadows", dlc.CastsShadows);
			//UI::Property("Soft Shadows", dlc.SoftShadows);
			UI::Property("Falloff", pointLightComponent.Falloff, 0.005f, 0.f, 1.f);
			UI::EndPropertyGrid();		
		}
	}

    void InspectorPanel::DrawMeshComponent(const MeshComponent& meshComponent) {
        if (UI::BeginTreeNode("Mesh Renderer", false))
		{
            auto mesh = meshComponent.Mesh;
            if (UI::BeginTreeNode("Materials"))
			{
                auto materials = mesh->GetMaterials();
                static uint32_t selectedMaterialIndex = 0;
                for(size_t i = 0; i < materials.size(); i ++) {
                    auto& material = materials[i];
                    std::string materialName = material->GetName();
                    if (materialName.empty())
                        materialName = "Unnamed Material #" + std::to_string(i);
                    ImGuiTreeNodeFlags node_flags = (selectedMaterialIndex == i ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
                    bool opened = ImGui::TreeNodeEx((void*)(&material), node_flags, materialName.c_str());
                    if (ImGui::IsItemClicked())
                    {
                        selectedMaterialIndex = i;
                    }
                    if (opened)
                        ImGui::TreePop();
                }
            ImGui::TreePop();
            }
        ImGui::TreePop();
        }
    }

	void InspectorPanel::DrawSkylightComponent(SkyLightComponent& skylightComponent) {
		if (ImGui::CollapsingHeader("Skylight", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text(skylightComponent.SceneEnvironment->FilePath.c_str());
			UI::BeginPropertyGrid();
			UI::Property("Level of Detail", skylightComponent.LOD, 0.01f,0.0f, 1.0f);
			UI::Property("Intensity", skylightComponent.Intensity, 0.01f, 0.0f, 5.0f);
			UI::EndPropertyGrid();
		}
	}

    void InspectorPanel::DrawTransformationComponent(TransformComponent& transformComponent) 
    {

        if (ImGui::CollapsingHeader("Transform", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
        {

            UI::ScopedStyle spacing (ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
			UI::ScopedStyle padding (ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

			ImGui::BeginTable("transformComponent", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
			ImGui::TableSetupColumn("label_column", 0, 100.0f);
			ImGui::TableSetupColumn("value_column", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip, ImGui::GetContentRegionAvail().x - 100.0f);

			ImGui::TableNextRow();
			DrawVec3Control("Translation", transformComponent.Translation);
			
			ImGui::TableNextRow();
			glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
			DrawVec3Control("Rotation", rotation);
			transformComponent.Rotation = glm::radians(rotation);

			ImGui::TableNextRow();
			DrawVec3Control("Scale", transformComponent.Scale, 1.0f);

			ImGui::EndTable();

			UI::ShiftCursorY(-8.0f);
			UI::Draw::Underline();

			UI::ShiftCursorY(18.0f);

            /*
            std::string id = "##Translation";
            ImGui::Text("Translation");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat3(id.c_str(), glm::value_ptr(transformComponent.Translation), 0.25f);

            ImGui::PopItemWidth();
            ImGui::NextColumn();
            
            ImGui::Text("Rotation");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat3("##rotation", glm::value_ptr(transformComponent.Rotation), 0.25f);


            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Text("Scale");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            glm::vec3 scale = transformComponent.Scale;
            ImGui::DragFloat3("##scale", glm::value_ptr(transformComponent.Scale), 0.25f);
            ImGui::PopItemWidth();
            */
        }
    }
}