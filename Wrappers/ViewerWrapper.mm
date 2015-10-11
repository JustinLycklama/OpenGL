//
//  ViewerWrapper.m
//  OpenGL-iOS
//
//  Created by Justin Lycklama on 2015-09-22.
//  Copyright © 2015 Justin Lycklama. All rights reserved.
//

#import "ViewerWrapper.h"
#import "Viewer.h"

@interface ViewerWrapper ()

@property (nonatomic, readonly) Viewer* cViewer;
@property (nonatomic, readonly) Camera* cCamera;

@end

@implementation ViewerWrapper
@synthesize cViewer = _cViewer;
@synthesize cCamera = _cCamera;

-(instancetype)initWithCamera:(CameraWrapper*)camera
{
	self = [super init];
	
	if(self)
	{
		_cViewer = new Viewer();
		[self setCamera:camera];
		
		_cViewer->setCamera(_cCamera);
		_cViewer->initialize();
	}
	
	return self;
}

- (void) dealloc
{
	free(_cViewer);
}

-(void)setCamera:(CameraWrapper*)camera
{
	//_cCamera = (Camera*)[camera camera];
	_cCamera = new Camera();
}

-(void)update:(CGFloat)timeElapsed
{
	_cViewer->update(timeElapsed);
}

-(void)render
{
	_cViewer->render();
}


@end