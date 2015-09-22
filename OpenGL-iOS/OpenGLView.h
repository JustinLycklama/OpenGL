//
//  OpenGLView.h
//  OpenGL-iOS
//
//  Created by Justin Lycklama on 2015-09-21.
//  Copyright © 2015 Justin Lycklama. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

@interface OpenGLView : UIView
{
	CAEAGLLayer* _eaglLayer;
	EAGLContext* _context;
	GLuint _colorRenderBuffer;
}


@end
