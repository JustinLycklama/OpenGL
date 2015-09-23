//
//  OpenGLView.m
//  OpenGL-iOS
//
//  Created by Justin Lycklama on 2015-09-21.
//  Copyright © 2015 Justin Lycklama. All rights reserved.
//

#import "OpenGLView.h"

// C++ Wrappers
#import "ViewerWrapper.h"
#import "CameraWrapper.h"

@interface OpenGLView()
{
	ViewerWrapper* _viewer;
}

@end

@implementation OpenGLView

#pragma mark - View Lifecycle

// Replace initWithFrame with this
- (id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	
	if (self)
	{
		[self setupLayer];
		[self setupContext];
		[self setupRenderBuffer];
		[self setupFrameBuffer];
		
		[self setupObjects];
		
		[self setupDisplayLink];
	}
	
	return self;
}

// Replace dealloc method with this
- (void)dealloc
{
	_context = nil;
}

#pragma mark - Class Methods

+ (Class)layerClass {
	return [CAEAGLLayer class];
}

#pragma mark - Setup

-(void)setupObjects
{
	
	CameraWrapper* camera = [[CameraWrapper alloc] init];
	_viewer = [[ViewerWrapper alloc] initWithCamera:camera];
	
	
	
//	Viewer* viewer = new Viewer();
//	Camera* camera = new Camera();
//
//	window->setCamera(camera);
//	window->setViewer(viewer);
//
//	viewer->setCamera(camera);
//	viewer->setWindow(window);
//
//	viewer->initialize();
}

- (void)setupLayer {
	_eaglLayer = (CAEAGLLayer*) self.layer;
	_eaglLayer.opaque = YES;
}

- (void)setupContext {
	EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES3;
	_context = [[EAGLContext alloc] initWithAPI:api];
	if (!_context) {
		NSLog(@"Failed to initialize OpenGLES 3.0 context");
		exit(1);
	}
 
	if (![EAGLContext setCurrentContext:_context]) {
		NSLog(@"Failed to set current OpenGL context");
		exit(1);
	}
}

- (void)setupRenderBuffer {
	glGenRenderbuffers(1, &_colorRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
	[_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}

- (void)setupFrameBuffer {
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							  GL_RENDERBUFFER, _colorRenderBuffer);
}

- (void)setupDisplayLink {
	CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
	[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)render:(CADisplayLink*)displayLink {
	glClearColor(255.0/255.0, 104.0/255.0, 55.0/255.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	[_viewer update:0.1];
	[_viewer render];
	
	[_context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
