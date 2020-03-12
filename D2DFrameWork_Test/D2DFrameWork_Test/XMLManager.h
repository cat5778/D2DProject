#pragma once

class CXMLManager
{
public:
	static CXMLManager* GetInstance();
	void DestroyInstance();

private:
	static CXMLManager* m_pInstance;

public:
	CXMLManager();
	~CXMLManager();
public:
	void CreateSimpleDoc(const char* _fileName)
	{

		tinyxml2::XMLDocument	doc;
		tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
		doc.LinkEndChild(decl);
		tinyxml2::XMLElement* root = doc.NewElement("Song");
		for (int i = 0; i < 3; i++)
		{

			tinyxml2::XMLElement* elemenet = doc.NewElement("Node");
			elemenet->SetAttribute("Pos", i);
			elemenet->SetAttribute("StartTime", i);
			elemenet->SetAttribute("EndTime", i);
			elemenet->SetAttribute("NodeType", i);
			//tinyxml2::XMLText* text = doc.NewText("world");
			//elemenet->LinkEndChild(text);
			root->LinkEndChild(elemenet);
			
		}
		doc.LinkEndChild(root);
		doc.SaveFile(_fileName);
	};
	void LoadData(const char* _fileName )
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError err;
		err = doc.LoadFile(_fileName);
		//std::cout << err << std::endl;
		tinyxml2::XMLElement*	pRootElement = doc.FirstChildElement();
		tinyxml2::XMLElement*	temp = pRootElement->FirstChildElement();
		while (0 != temp)
		{
			tinyxml2::XMLElement* temp2 = temp;
			std::string name = temp2->Name();

			if (name.compare("Song"))
			{
				 temp2->FloatAttribute("Pos") ;
				 temp2->FloatAttribute("StartTime");
				 temp2->FloatAttribute("EndTime");
				 temp2->IntAttribute("NodeType");

			}
			temp = (tinyxml2::XMLElement*)temp->NextSiblingElement();
		}
	}
};

