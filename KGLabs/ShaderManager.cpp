#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	monThread = nullptr;
}

void ShaderManager::AddShader(Shader* shader)
{
	auto it = programs.find(shader);
	if (it != programs.end())
		return;

	programs[shader] = shader->getPaths();
	last_times[shader] = shader->getLastTimes();

}

void ShaderManager::DeleteShader(Shader* shader)
{
	programs.erase(shader);
	
}

void ShaderManager::Start()
{
	if (monThread)
		return;
	monThread = new std::thread(&ShaderManager::Monitor, this);
}

void ShaderManager::Reload()
{
	for (auto reloadable : programsToReload)
	{
		reloadable.program->load(reloadable.filepath, reloadable.shaderType);
	}
	programsToReload.clear();
}

ShaderManager& ShaderManager::getInstance()
{
	static ShaderManager shaderManager;
	return shaderManager;
}

void ShaderManager::Monitor()
{
	printf("SHADER MANAGER: Started.\n");
	while (1)
	{
		for (auto it = programs.begin(); it != programs.end(); it++)
			CheckLastTimes(it->first);
		
		// Check one time in a second
		Sleep(1000);
	}
}

INT64 ShaderManager::GetShaderLastTime(std::string filepath)
{
	// Open the file
	HANDLE hFile = CreateFileA(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "SHADER MANAGER: Can't get shader last time\n";
		return 0;
	}
	// Get time
	FILETIME fTime;
	GetFileTime(hFile, NULL, NULL, &fTime);
	INT64 result = (INT64)fTime.dwLowDateTime | ((INT64)fTime.dwHighDateTime) << 32;
	//std::cout << "Last time is " << result << std::endl;

	// Close the file
	CloseHandle(hFile);

	return result;

}

// TODO: ADD OTHER TYPES OF SHADER
void ShaderManager::CheckLastTimes(Shader* shader)
{
	auto sIt = programs.find(shader);
	if (sIt == programs.end())
		return;

	std::map<GLuint, std::string>* fPaths = sIt->second;

	const int sCount = 2;
	GLuint types[sCount] =
	{
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER
	};

	for (int i = 0; i < sCount; i++)
	{
		// Try to get file path
		auto it = fPaths->find(types[i]);
		if (it == fPaths->end())
			continue;
		// Get last time
		std::string filepath = it->second;
		INT64 last_time = GetShaderLastTime(filepath);

		// And check it and update
		auto sLastTimes = last_times[shader];
		auto iLastTime = sLastTimes->find(i);
		if (iLastTime != sLastTimes->end())
		{
			if (iLastTime->second != last_time)
			{
				programsToReload.push_back(
					Reloadable{shader, filepath, types[i]}
				);
			}
		}

		(*sLastTimes)[i] = last_time;
	}
}
