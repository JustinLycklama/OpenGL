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
	CameraWrapper* _camera;
	
	CGFloat _lastTime;
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
		[self setupDepthBuffer];
		
		[self setupObjects];
		
		[self setupDisplayLink];
		_lastTime = [[NSDate date] timeIntervalSince1970];
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
	_camera = [[CameraWrapper alloc] init];
	_viewer = [[ViewerWrapper alloc] initWithCamera:_camera];
	
	[_camera setAspectRatio:(self.frame.size.width / self.frame.size.height)];
	//[camera setAspectRatio:(1.0/1.0)];
	
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
	
	NSLog(@"Current GL Context %s", glGetString(GL_VERSION));
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

-(void)setupDepthBuffer
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
 
	glDepthRangef(0.0,10.0);
	glEnable(GL_DEPTH_TEST);
}

- (void)setupDisplayLink {
	CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
	[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
	glViewport(0, 0, self.frame.size.width, self.frame.size.height);
}

- (void)render:(CADisplayLink*)displayLink {
	glClearColor(100.0/255.0, 104.0/255.0, 55.0/255.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//glViewport(0, 0, self.frame.size.width, self.frame.size.height);

	CGFloat thisTime = [[NSDate date] timeIntervalSince1970];
	CGFloat secondsElapsed = thisTime - _lastTime;
	_lastTime = thisTime;
	
	[_viewer update:secondsElapsed];
	[_viewer render];
 
//	// 2
//	glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE,
//        sizeof(Vertex), 0);
//	glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE,
//        sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
// 
//	// 3
//	glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]),
	
	[_context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
