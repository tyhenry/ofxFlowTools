
#pragma once

#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {
	
	class ftAgeLifespanMassSizeParticleShader : public ftShader {
	public:
		ftAgeLifespanMassSizeParticleShader() {
            bInitialized = 1;
			if (ofIsGLProgrammableRenderer()) { glFour(); } else { glTwo(); }
			string shaderName = "ftAgeLifespanMassSizeParticleShader";
			if (bInitialized) { ofLogVerbose(shaderName + " initialized"); }
			else { ofLogWarning(shaderName + " failed to initialize"); }
		}
		
	protected:
		void glTwo() {
			fragmentShader = GLSL120(uniform sampler2DRect Backbuffer;
									 uniform sampler2DRect Position;
									 uniform sampler2DRect Velocity;
									 uniform sampler2DRect Density;
									 uniform sampler2DRect Obstacle;
									 uniform vec2  Scale;
									 uniform float GlobalTime;
									 uniform float DeltaTime;
									 uniform float BirthChance;
									 uniform float BirthVelocityChance;
									 uniform float LifeSpan;
									 uniform float LifeSpanSpread;
									 uniform float Mass;
									 uniform float MassSpread;
									 uniform float Size;
									 uniform float SizeSpread;
									 
									 // hash based 3d value noise
									 float random(float p) {
										 return fract(sin(p)*13420.123);
									 }
									 
									 float noise(vec2 p) {
										 return random(p.x + sin(p.y*112.5345));
									 }
									 
									 void main(){
										 vec2 st = gl_TexCoord[0].st;
										 vec2 st2 = st * Scale;
										 
										 vec4 alms = texture2DRect(Backbuffer, st);
										 float p_age = alms.x;
										 float p_life = alms.y;
										 float p_mass = alms.z;
										 float p_size = alms.w;
										 
										 if (p_age > 0.0) {
											 p_age += DeltaTime;
										 }
										 
										 if (p_age == 0.0) {
											 float birthRandom = noise( st * GlobalTime + 304.5) / BirthChance;
											 float speed = length(texture2DRect(Velocity, st2).rg / Scale);
											 float birthFromVelocity = speed * BirthVelocityChance;
											 if (birthRandom > 0.001 && birthRandom < birthFromVelocity ) {
												 p_age = 0.001;
												 float lifeRandom =  noise( st * GlobalTime + 137.34) * 2.0 - 1.0;
												 p_life = LifeSpan + LifeSpan * LifeSpanSpread * lifeRandom;
												 float massRandom =  noise( st * GlobalTime + 281.05) * 2.0 - 1.0;
												 p_mass =  Mass + Mass * MassSpread * massRandom;
												 float sizeRandom =   noise( st * GlobalTime + 431.93) * 2.0 - 1.0;
												 p_size =  Size + Size * SizeSpread * sizeRandom;
											 }
										 }
										 
										 if (p_age > p_life ) {
											 p_age = 0.0;
										 }
										 
										 vec2 particlePos = texture2DRect(Position, st).xy;
										 particlePos *= Scale;
										 float inverseSolid = 1.0 - ceil(texture2DRect(Obstacle, particlePos).x - 0.5);
										 p_age *= inverseSolid;
										 
										 gl_FragColor = vec4(p_age, p_life, p_mass, p_size);
									 }
									 );
			
			bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= linkProgram();
		}
		
		void glFour() {
			fragmentShader = GLSL410(uniform sampler2DRect Backbuffer;
									 uniform sampler2DRect Position;
									 uniform sampler2DRect Velocity;
									 uniform sampler2DRect Density;
									 uniform sampler2DRect Obstacle;
									 uniform vec2	Scale;
									 uniform float GlobalTime;
									 uniform float DeltaTime;
									 uniform float BirthChance;
									 uniform float BirthVelocityChance;
									 uniform float LifeSpan;
									 uniform float LifeSpanSpread;
									 uniform float Mass;
									 uniform float MassSpread;
									 uniform float Size;
									 uniform float SizeSpread;
									 
									 in vec2 texCoordVarying;
									 out vec4 fragColor;
									 
									 // hash based 3d value noise
									 float random(float p) {
										 return fract(sin(p)*13420.123);
									 }
									 
									 float noise(vec2 p) {
										 return random(p.x + sin(p.y*112.5345));
									 }
									 
									 void main(){
										 vec2 st = texCoordVarying;
										 vec2 st2 = st * Scale;
										 
										 vec4 alms = texture(Backbuffer, st);
										 float p_age = alms.x;
										 float p_life = alms.y;
										 float p_mass = alms.z;
										 float p_size = alms.w;
										 
										 if (p_age > 0.0) {
											 p_age += DeltaTime;
										 }
										 
										 if (p_age == 0.0) {
											 float birthRandom = noise( st * GlobalTime + 304.5) / BirthChance;
											 float speed = length(texture(Velocity, st2).rg / Scale);
											 float birthFromVelocity = speed * BirthVelocityChance;
											 if (birthRandom > 0.001 && birthRandom < birthFromVelocity ) {
												 p_age = 0.001;
												 float lifeRandom =  noise( st * GlobalTime + 137.34) * 2.0 - 1.0;
												 p_life = LifeSpan + LifeSpan * LifeSpanSpread * lifeRandom;
												 float massRandom =  noise( st * GlobalTime + 281.05) * 2.0 - 1.0;
												 p_mass =  Mass + Mass * MassSpread * massRandom;
												 float sizeRandom =   noise( st * GlobalTime + 431.93) * 2.0 - 1.0;
												 p_size =  Size + Size * SizeSpread * sizeRandom;
											 }
										 }
										 
										 if (p_age > p_life ) {
											 p_age = 0.0;
										 }
										 vec2 particlePos = texture(Position, st).xy;
										 particlePos *= Scale;
										 float inverseSolid = 1.0 - ceil(texture(Obstacle, particlePos).x - 0.5);
										 p_age *= inverseSolid;
										 
										 fragColor = vec4(p_age, p_life, p_mass, p_size);
									 }
									 );
			
			bInitialized *= setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
			bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= bindDefaults();
			bInitialized *= linkProgram();
		}
		
	public:
		void update(ofFbo& _fbo, ofTexture& _backTex, ofTexture& _posTex, ofTexture& _velTex, ofTexture& _denTex, ofTexture& _obstacleTexture,
					float _deltaTime, float _birthChance, float _birthVelocityChance, float _lifeSpan, float _lifeSpanSpread, float _mass, float _massSpread, float _size, float _sizeSpread){
			
			_fbo.begin();
			ofClear(0,0);
			begin();
			setUniformTexture("Backbuffer", _backTex, 0);
			setUniformTexture("Position", _posTex, 1);
			setUniformTexture("Velocity", _velTex, 2);
			setUniformTexture("Density", _denTex, 3);
			setUniformTexture("Obstacle", _obstacleTexture, 4);
			setUniform2f("Scale", _velTex.getWidth() / _fbo.getWidth(), _velTex.getHeight()/ _fbo.getHeight());
			float modTime = 64.f; // little hack to prevent particle lines ( the random function in the shader does not behave as expected)
			setUniform1f("GlobalTime", modf(ofGetElapsedTimef(), &modTime));
			setUniform1f("DeltaTime", _deltaTime);
			setUniform1f("BirthChance", _birthChance * _deltaTime);
			setUniform1f("BirthVelocityChance", _birthVelocityChance);
			setUniform1f("LifeSpan", _lifeSpan);
			setUniform1f("LifeSpanSpread", _lifeSpanSpread);
			setUniform1f("Mass", _mass);
			setUniform1f("MassSpread", _massSpread);
			setUniform1f("Size", _size);
			setUniform1f("SizeSpread", _sizeSpread);
			renderFrame(_fbo.getWidth(), _fbo.getHeight());
			end();
			_fbo.end();
		}
	};
}


