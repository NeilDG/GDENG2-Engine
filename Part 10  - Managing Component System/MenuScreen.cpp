#include "MenuScreen.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <iostream>
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "VertexShader.h"
#include "ShaderLibrary.h"
#include "SceneWriter.h"
#include "SceneReader.h"
#include "Debug.h"

MenuScreen::MenuScreen(): AUIScreen("MenuScreen")
{
	this->openSceneDialog = new ImGui::FileBrowser();
	this->openSceneDialog->SetTitle("Open Scene");
	this->openSceneDialog->SetTypeFilters({ ".iet"});

	this->saveSceneDialog = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
	this->saveSceneDialog->SetTitle("Save Scene");
	this->saveSceneDialog->SetTypeFilters({ ".iet" });
}

MenuScreen::~MenuScreen()
{
	delete this->openSceneDialog;
	delete this->saveSceneDialog;
}

void MenuScreen::drawUI()
{

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open..", "Ctrl+O")) {
				this->openSceneDialog->Open();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				this->saveSceneDialog->Open();
			}
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
				this->saveSceneDialog->Open();
			}
			if (ImGui::MenuItem("Exit Editor", "Ctrl+W")) {/*Do something */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game Object")) {
			if (ImGui::MenuItem("Create Sphere")) { this->OnCreateSphereClicked(); }
			if (ImGui::MenuItem("Create Cube")) { this->OnCreateCubeClicked(); }
			if (ImGui::MenuItem("Create Textured Cube")) { this->OnCreateTexturedCubeClicked(); }
			if (ImGui::MenuItem("Create Placeholder Physics Cube")) { this->OnPhysicsCubeClicked(); }
			if (ImGui::MenuItem("Create Plane")) { this->OnCreatePlaneClicked(); }
			if (ImGui::MenuItem("Create Placeholder Physics Plane")) { this->OnPhysicsPlaneClicked(); }
			if (ImGui::BeginMenu("Light")) {
				if (ImGui::MenuItem("Point Light")) { /* Do stuff */ }
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Components")) {
			if (ImGui::MenuItem("Rigid Body")) { this->OnRigidBodyComponentClicked(); }
			if (ImGui::MenuItem("Material")) { this->OnMaterialComponentClicked(); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	this->openSceneDialog->Display();
	this->saveSceneDialog->Display();

	if (this->saveSceneDialog->HasSelected())
	{
		SceneWriter writer = SceneWriter(this->saveSceneDialog->GetSelected().string());
		writer.writeToFile();

		this->saveSceneDialog->ClearSelected();
		this->saveSceneDialog->Close();
	}
	
	else if (this->openSceneDialog->HasSelected()) {
		SceneReader reader = SceneReader(this->openSceneDialog->GetSelected().string());
		reader.readFromFile();

		this->openSceneDialog->ClearSelected();
		this->openSceneDialog->Close();
	}
}

void MenuScreen::OnCreateCubeClicked()
{
	//initialize vertex for object
	GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::CUBE);
}

void MenuScreen::OnCreateTexturedCubeClicked()
{
	GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::TEXTURED_CUBE);
}

void MenuScreen::OnCreateSphereClicked()
{
	std::cout << "Creating sphere placeholder. \n";
}

void MenuScreen::OnCreatePlaneClicked()
{
	//initialize vertex for object
	GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::PLANE);
}

void MenuScreen::OnRigidBodyComponentClicked()
{
	Debug::Log("Creating rigid body placeholder");
}

void MenuScreen::OnMaterialComponentClicked()
{
	Debug::Log("Creating material placeholder.");
}

void MenuScreen::OnPhysicsCubeClicked()
{
	for (int i = 0; i < 20; i++) {
		GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::PHYSICS_CUBE);
	}
}

void MenuScreen::OnPhysicsPlaneClicked()
{
	GameObjectManager::getInstance()->createObject(AGameObject::PrimitiveType::PHYSICS_PLANE);
}
