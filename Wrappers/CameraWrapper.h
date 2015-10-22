//
//  CameraWrapper.h
//  OpenGL-iOS
//
//  Created by Justin Lycklama on 2015-09-22.
//  Copyright © 2015 Justin Lycklama. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface CameraWrapper : NSObject

-(void*)camera;
-(void)setAspectRatio:(CGFloat)ratio;

@end