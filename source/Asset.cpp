
// standard C++ libraries
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing fla

#include "Asset.h"
#include "Mesh.h"

const bool FORCE_RELOAD = true;

Asset::Asset(Shape s, Texture* tex, Program* prog)
{
	program = prog;
	texture = tex;
	shape = s;

	switch(shape){
		case Triangle:
			LoadTriangle();
		break;
		case Cube:
			LoadCube();
		break;
	}
}

Asset::Asset(string mesh, Texture* tex, Texture* bmp, Program* prog)
{
	program = prog;
	texture = tex;
	normalMap = bmp;

	loadMesh(mesh, ".obj");
}

Asset::Asset(string assetPath, Program* prog)
{
	program = prog;
	
	loadFBXFile(assetPath.c_str());
}

Asset::~Asset()
{

}

vec3 Asset::getSpecularColor() {
	return specularColor;
}

void Asset::setSpecularColor(vec3 col) {
	specularColor = col;
}

void Asset::render(){
	if(program->hasTextures()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getId());

		glUniform1i(program->getUniformLocation("materialTex"), 0);
	}
	if(program->hasBumpMapping()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap->getId());

		glUniform1i(program->getUniformLocation("normalMap"), 1);
	}

	glUniform1f(program->getUniformLocation("materialShininess"), shininess);
	glUniform3fv(program->getUniformLocation("materialSpecularColor"), 1, glm::value_ptr(specularColor));

    // bind the VAO
    glBindVertexArray(gVAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Asset::update(float secondsElapsed) {
}

void Asset::LoadTriangle() {
    // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Put the three triangle verticies into the VBO
     GLfloat vertexData[] = {
        //  X     Y     Z       U     V
         0.0f, 0.8f, 0.0f,   0.5f, 1.0f,
        -0.8f,-0.8f, 0.0f,   0.0f, 0.0f,
         0.8f,-0.8f, 0.0f,   1.0f, 0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = program->getAttributeLocation("vert");
	GLuint vertTex = program->getAttributeLocation("vertTexCoord");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(vertTex);
    glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Set vertex count
	vertexCount = 3;
}


void Asset::LoadCube() {
	 // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	GLfloat vertexData[] = {
        //  X     Y     Z       U     V          Normal
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,

        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,

        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,

        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,

        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,

        // right
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = program->getAttributeLocation("vert");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	if(program->hasTextures()) {
		GLuint vertTex = program->getAttributeLocation("vertTexCoord");
		
		glEnableVertexAttribArray(vertTex);
		glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  8*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
	}

	// connect the Normal coords to the "vertNormalCoord" attribute of the vertex shader
	GLuint vertNormal = program->getAttributeLocation("vertNormal");

    glEnableVertexAttribArray(vertNormal);
    glVertexAttribPointer(vertNormal, 3, GL_FLOAT, GL_TRUE,  8*sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Set vertex count
	vertexCount = 36;
}

void Asset::loadMesh(string fileName, string type) {
	 
	FILE * pFile;
	long sizeOf;
	int pointsPerVertex = 11;
	GLfloat* vertexData;
	size_t result;	
	
	if (type == ".obj")
	{
		pFile = fopen ((fileName + ".data").c_str(), "rb");

		// If no .data file exists, load a .obj and create a .data	
		if (pFile==NULL || FORCE_RELOAD) {
			
			Mesh* mesh = new Mesh(fileName);

			vertexData = mesh->getVertexData();
			sizeOf = sizeof(GLfloat) * mesh->getVertexCount() * mesh->getPointsPerVertex();
				
			free(mesh);
		}
		// Otherwise load the .data file
		else {
			// obtain file size:
			fseek (pFile , 0 , SEEK_END);
			sizeOf = ftell (pFile);
			rewind (pFile);

			// allocate memory to contain the whole file:
			vertexData = (GLfloat*) malloc (sizeof(char)*sizeOf);
			if (vertexData == NULL) { throw std::runtime_error("Memory Failure in Loading Mesh .data"); }

			// copy the file into the buffer:
			result = fread (vertexData,1,sizeOf,pFile);
			if (result != sizeOf) { throw std::runtime_error("Reading error in Loading Mesh .data"); }

			fclose (pFile);
		}
	} else if (type == ".fbx") // NOT USED. USING THE FBX LOAD METHOD
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile( Program::ResourcePath(fileName, ".fbx").c_str(),
												 aiProcess_CalcTangentSpace       |
												 aiProcess_Triangulate            |
												 aiProcess_JoinIdenticalVertices  |
												 aiProcess_SortByPType);
		
		if(!scene)
		{
			return;
		}
		
		aiMesh* aimesh = scene->mMeshes[0];
		
		Mesh* mesh = new Mesh(aimesh);
		
		vertexData = mesh->getVertexData();
		sizeOf = sizeof(GLfloat) * mesh->getVertexCount() * mesh->getPointsPerVertex();
		
		free(mesh);
	}

	// make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeOf, vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = program->getAttributeLocation("vert");

    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, pointsPerVertex*sizeof(GLfloat), NULL);
    
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	if(program->hasTextures() || program->hasBumpMapping()) {
		GLuint vertTex = program->getAttributeLocation("vertTexCoord");
		
		glEnableVertexAttribArray(vertTex);
		glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
	}

	// connect the Normal coords to the "vertNormalCoord" attribute of the vertex shader
	GLuint vertNormal = program->getAttributeLocation("vertNormal");

    glEnableVertexAttribArray(vertNormal);
    glVertexAttribPointer(vertNormal, 3, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

	if(program->hasBumpMapping()) {
		// connect the Tangent coords to the "vertNormalCoord" attribute of the vertex shader
		GLuint vertTangent = program->getAttributeLocation("vertTangent");

		glEnableVertexAttribArray(vertTangent);
		glVertexAttribPointer(vertTangent, 3, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(8 * sizeof(GLfloat)));
	}

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Set vertex count
	vertexCount = sizeOf / (sizeof(GLfloat) * pointsPerVertex);
}


void Asset::buildNodeNameMap(aiNode* node)
{
	string nodeName = node->mName.data;
	if (boneNameMap.find(nodeName) != boneNameMap.end() || nodeAnimationMap.find(nodeName) != nodeAnimationMap.end())
	{
		nodeNameMap[node->mName.data] = node;
	}
	
	if (node->mNumChildren > 0)
	{
		for (int child = 0; child < node->mNumChildren; child++)
		{
			buildNodeNameMap(node->mChildren[child]);
		}
	}
}

void Asset::loadFBXFile(const char* filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( Program::ResourcePath(filePath, ".fbx").c_str(),
											 aiProcess_CalcTangentSpace       |
											 aiProcess_Triangulate            |
											 aiProcess_JoinIdenticalVertices  |
											 aiProcess_SortByPType);
	
	if(!scene)
	{
		
		return;
	}
	
	// First and probably only mesh
	const aiMesh* mesh = scene->mMeshes[1];
	int numVerts = mesh->mNumFaces*3;
	
	// 3 point for each vertetx
	// 2 points for the tex coordinates
	// 3 points for the nomral data
	// 3 unused
	// 1 for bone id
	
	int currentBoneID = 0;
	int row = 0;

	int pointsPerVertex = (3 + 2 + 3 + 3 + 1);
	long sizeOf = sizeof(GLfloat) * numVerts * pointsPerVertex;
	
	int bufferSize = numVerts * pointsPerVertex;
	GLfloat* vertexData = new GLfloat[bufferSize];
	
	// Load
	
	
	/* I'm first going to need to look through each bone in this mesh. theres 36ish
		I need to store each bone, and its weights in a table.
	 
	 Then when looking through the nodes here, recursively, if any of the nodes match one of the names I need to save the transforms up until that point
	 
	 This information above can probably be done once, and then saved
	 
	 Then, when looking through the verticies, I have to look through all bones to see if the vertex is affected, and update accodingly, based on it's weight.
	
	 
	 refer to
	 http://gamedev.stackexchange.com/questions/26382/i-cant-figure-out-how-to-animate-my-loaded-model-with-assimp
	 https://www.khronos.org/opengl/wiki/Skeletal_Animation
	 https://en.wikipedia.org/wiki/Skeletal_animation
	 */
	
	
	// Build boneNameMap
	for (int boneId = 0; boneId < mesh->mNumBones; boneId++)
	{
		aiBone* bone = mesh->mBones[boneId];
		
		boneNameMap[bone->mName.data] = bone;
		
		// Build an association from each vertex to each bone. 'vertexBoneWeightsMap'
		for (int weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++)
		{
			aiVertexWeight weight = bone->mWeights[weightIndex];
			
			int vertexId = weight.mVertexId;
			pair<float, int> weightBonePair = pair<float, int>(weight.mWeight, boneId);
			
			// Add to list, or create a new list
			if (vertexBoneWeightsMap.find(vertexId) != vertexBoneWeightsMap.end())
			{
				vertexBoneWeightsMap[vertexId].push_back(weightBonePair);
			} else
			{
				VertexBoneWeights newWeights;
				newWeights.push_back(weightBonePair);
				vertexBoneWeightsMap[vertexId] = newWeights;
			}
		}
	}
	
	// Hard code a load of the walking fox animation, and try to step through it to get bone transformation data
	Assimp::Importer importer2; // Second importer, noticing odd things on first scene when using old importer
	const aiScene* walkScene = importer2.ReadFile( Program::ResourcePath("Fox_Walk", ".fbx").c_str(),
												  aiProcess_CalcTangentSpace       |
												  aiProcess_Triangulate            |
												  aiProcess_JoinIdenticalVertices  |
												  aiProcess_SortByPType);
	
	if(!walkScene)
	{
		return;
	}
	
	aiAnimation* animation = walkScene->mAnimations[0];
	
	duration = animation->mDuration;
	ticksPerSecond = animation->mTicksPerSecond;
	
	// Get the flat list of node animations. This contains the transformation data at different ticks in time
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* nodeAnimation = animation->mChannels[i];
		nodeAnimationMap[nodeAnimation->mNodeName.data] = nodeAnimation;
	}
	
	// With our bone mapping and animation node mapping, Check all nodes in the scene, and find the ones that match either map
	// If we find a match, we now have the heiarchy for that bone / animation
	buildNodeNameMap(scene->mRootNode);

	// Finally, build the vertex data
	for(unsigned int i=0;i<mesh->mNumFaces;i++)
	{
		const aiFace& face = mesh->mFaces[i];
		
		for(int j=0;j<3;j++)
		{
			int vertexIndex = face.mIndices[j];
			
			aiVector3D vert = mesh->mVertices[vertexIndex];
			
			vertexData[row + 0] = vert.x;
			vertexData[row + 1] = vert.y;
			vertexData[row + 2] = vert.z;
			
			aiVector3D uv = mesh->mTextureCoords[0][vertexIndex];
			
			vertexData[row + 3] = uv.x; // u
			vertexData[row + 4] = uv.y; // v
			
			aiVector3D normal = mesh->mNormals[vertexIndex];
			
			vertexData[row + 5] = normal.x;
			vertexData[row + 6] = normal.y;
			vertexData[row + 7] = normal.z;
			
			vertexData[row + 8] = 0;
			vertexData[row + 9] = 0;
			vertexData[row + 10] = 0;
			
			vertexData[row + 11] = vertexBoneWeightsMap[vertexIndex][0].second; // Set to bone index. Just choose a single index for now, ignore bone weights
			
			row += 12; //temp hardcode, should get num point per index
		}
	}
	
	
	// make and bind the VAO
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
	
	// make and bind the VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeOf, vertexData, GL_STATIC_DRAW);
	
	// connect the xyz to the "vert" attribute of the vertex shader
	GLuint vert = program->getAttributeLocation("vert");
	
	glEnableVertexAttribArray(vert);
	glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, pointsPerVertex*sizeof(GLfloat), NULL);
	
	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	if(program->hasTextures() || program->hasBumpMapping()) {
		GLuint vertTex = program->getAttributeLocation("vertTexCoord");
		
		glEnableVertexAttribArray(vertTex);
		glVertexAttribPointer(vertTex, 2, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
	}
	
	// connect the Normal coords to the "vertNormalCoord" attribute of the vertex shader
	GLuint vertNormal = program->getAttributeLocation("vertNormal");
	
	glEnableVertexAttribArray(vertNormal);
	glVertexAttribPointer(vertNormal, 3, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));
	
	if(program->hasBumpMapping()) {
		// connect the Tangent coords to the "vertNormalCoord" attribute of the vertex shader
		GLuint vertTangent = program->getAttributeLocation("vertTangent");
		
		glEnableVertexAttribArray(vertTangent);
		glVertexAttribPointer(vertTangent, 3, GL_FLOAT, GL_TRUE,  pointsPerVertex*sizeof(GLfloat), (const GLvoid*)(8 * sizeof(GLfloat)));
	}
	
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	// Set vertex count
	vertexCount = sizeOf / (sizeof(GLfloat) * pointsPerVertex);
}
