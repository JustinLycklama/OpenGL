//
//  CameraWrapper.m
//  OpenGL-iOS
//
//  Created by Justin Lycklama on 2015-09-22.
//  Copyright © 2015 Justin Lycklama. All rights reserved.
//

#import "CameraWrapper.h"
#import "Camera.h"

@interface CameraWrapper ()

@property (nonatomic, readonly) Camera* cCamera;

@end

@implementation CameraWrapper
@synthesize cCamera = _cCamera;

-(instancetype)init
{
	self = [super init];
	
	if(self)
	{
		_cCamera = new Camera();
	}
	
	return self;
}

- (void)dealloc
{
	free(_cCamera);
}

-(void*)camera
{	
	return _cCamera;
}

-(void)setAspectRatio:(CGFloat)ratio
{
	_cCamera->setAspectRatio(ratio);
}

@end