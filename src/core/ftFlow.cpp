//
//  ftFlow.cpp
//  example_core
//
//  Created by Ties East on 28/06/2018.
//

#include "ftFlow.h"

namespace flowTools {
	
	void ftFlow::allocate(int _inputWidth, int _inputHeight, GLint _inputInternalFormat, int _outputWidth, int _outputHeight, GLint _outputInternalFormat) {
		inputWidth = _inputWidth;
		inputHeight = _inputHeight;
		inputInternalFormat = _inputInternalFormat;
		outputWidth = _outputWidth;
		outputHeight = _outputHeight;
		outputInternalFormat = _outputInternalFormat;
		
		inputFbo.allocate(inputWidth, inputHeight, inputInternalFormat);
		ftUtil::zero(inputFbo);
		
		outputFbo.allocate(outputWidth, outputHeight, outputInternalFormat);
		ftUtil::zero(outputFbo);
		
		visualizationField.setup(outputWidth, outputHeight);
	}
	
	void ftFlow::resize(int _inputWidth, int _inputHeight, int _outputWidth, int _outputHeight) {
		allocate(_inputWidth, _inputHeight, inputInternalFormat, _outputWidth, _outputHeight, outputInternalFormat);
	}
	
	void ftFlow::add(ftPingPongFbo &_dstFbo, ofTexture &_srcTex, float _strength) {
		// check for required GL_TEXTURE_RECTANGLE texture target and report / abort if not
		const auto texTarget = _srcTex.getTextureData().textureTarget;
		if (texTarget != GL_TEXTURE_RECTANGLE) {
			ofLogError( "ofxFlowTools" ) << "[ " << typeid(*this).name() << "::add ] requires input texture to use textureTarget: GL_TEXTURE_RECTANGLE, but texture is using: " << (texTarget == GL_TEXTURE_2D ? "GL_TEXTURE_2D" : ofToString(texTarget));
			return;
		}
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		_dstFbo.swap();
		addMultipliedShader.update(_dstFbo.get(), _dstFbo.getBackTexture(), _srcTex, 1.0, _strength);
		ofPopStyle();
	}
	
	void ftFlow::set(ftPingPongFbo &_dstFbo, ofTexture &_srcTex) {
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		ftUtil::zero(_dstFbo);
		ftUtil::stretch(_dstFbo.get(), _srcTex);
		ofPopStyle();
	}
};

