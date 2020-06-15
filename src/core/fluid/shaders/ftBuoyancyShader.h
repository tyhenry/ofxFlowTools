
#pragma once

#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {
	
	class ftBuoyancyShader : public ftShader {
	public:
		ftBuoyancyShader() {
			bInitialized = 1;
			if (ofIsGLProgrammableRenderer()) { glFour(); } else { glTwo(); }
			string shaderName = "ftBuoyancyShader";
			if (bInitialized) { ofLogVerbose(shaderName + " initialized"); }
			else { ofLogWarning(shaderName + " failed to initialize"); }
//			load("tempShader/ftVertexShader.vert", "tempShader/" + shaderName + ".frag");
		}
		
	protected:
		void glTwo() {
			fragmentShader = GLSL120(
									 uniform sampler2DRect tex_velocity;
									 uniform sampler2DRect tex_temperature;
									 uniform sampler2DRect tex_density;

									 uniform vec2 tex_vel_scale;
									 uniform vec2 tex_tem_scale;
									 uniform vec2 tex_den_scale;
									 
									 uniform float temperature_ambient;
									 uniform float timestep;
									 uniform float fluid_buoyancy;
									 uniform float fluid_weight;
									 
									 void main(){
										 vec2 st = gl_TexCoord[0].st;
										 
										 vec2  velocity    = texture2DRect(tex_velocity   , st * tex_vel_scale).xy;
										 float temperature = texture2DRect(tex_temperature, st * tex_tem_scale).x;
										 
										 float dtemp = temperature - temperature_ambient;
										 vec2 buoyancy = vec2(0.0);
										 if (dtemp != 0.0) {
											 float density = texture2DRect(tex_density, st * tex_den_scale).a;
											 float buoyancy_force = timestep * dtemp * fluid_buoyancy - density * fluid_weight;
											 buoyancy = vec2(0, -1) * buoyancy_force;
										 }
										 
										 gl_FragColor = vec4(buoyancy, 0.0, 0.0);;
									 }
									 );
			
			bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= linkProgram();
		}
		
		void glFour() {
			fragmentShader = GLSL410(
									 precision mediump float;
									 precision mediump int;
									 
									 in vec2 texCoordVarying;
									 out vec2 glFragColor;
									 
									 uniform sampler2DRect tex_velocity;
									 uniform sampler2DRect tex_temperature;
									 uniform sampler2DRect tex_density;

									 uniform vec2 tex_vel_scale;
									 uniform vec2 tex_tem_scale;
									 uniform vec2 tex_den_scale;
									 
									 uniform float temperature_ambient;
									 uniform float timestep;
									 uniform float fluid_buoyancy;
									 uniform float fluid_weight;
									 
									 void main(){
										 vec2 st = texCoordVarying;
										 
										 vec2  velocity    = texture(tex_velocity   , st * tex_vel_scale).xy;
										 float temperature = texture(tex_temperature, st * tex_tem_scale).x;
										 
										 float dtemp = temperature - temperature_ambient;
										 vec2 buoyancy = vec2(0.0);
										 if (dtemp != 0.0) {
											 float density = texture(tex_density, st * tex_den_scale).a;
											 float buoyancy_force = timestep * dtemp * fluid_buoyancy - density * fluid_weight;
											 buoyancy = vec2(0, -1) * buoyancy_force;
										 }
										 
										 glFragColor = buoyancy;
									 }
									 );
			
			bInitialized *= setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
			bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= bindDefaults();
			bInitialized *= linkProgram();
		}
		
	public:
		void update(ofFbo& _fbo, ofTexture& _velTex, ofTexture& _temTex, ofTexture _denTex, float _timeStep, float _ambientTemperature, float _buoyancy, float _weight){
			_fbo.begin();
			begin();
			setUniform1f		("temperature_ambient", _ambientTemperature);
			setUniform1f		("timestep",			_timeStep);
			setUniform1f		("fluid_buoyancy",		_buoyancy);
			setUniform1f		("fluid_weight",		_weight);
			setUniformTexture	("tex_velocity",		_velTex,	0);
			setUniformTexture	("tex_temperature",		_temTex,	1);
			setUniformTexture	("tex_density",			_denTex,	2);
			setUniform2f		("tex_vel_scale",		_velTex.getWidth() / _fbo.getWidth(), _velTex.getHeight() / _fbo.getHeight());
			setUniform2f		("tex_tem_scale",		_temTex.getWidth() / _fbo.getWidth(), _temTex.getHeight() / _fbo.getHeight());
			setUniform2f		("tex_den_scale",		_denTex.getWidth() / _fbo.getWidth(), _denTex.getHeight() / _fbo.getHeight());
			renderFrame(_fbo.getWidth(), _fbo.getHeight());
			end();
			_fbo.end();
		}
	};
}



