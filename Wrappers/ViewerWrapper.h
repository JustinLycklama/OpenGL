//
//  ViewerWrapper.hh
//  OpenGL-iOS
//
//  Created by Justin Lycklama on 2015-09-22.
//  Copyright © 2015 Justin Lycklama. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#import "CameraWrapper.h"

@interface ViewerWrapper : NSObject

@property (nonatomic, copy) NSString* string;

-(instancetype)initWithCamera:(CameraWrapper*)camera NS_DESIGNATED_INITIALIZER;

-(void)update:(CGFloat)timeElapsed;
-(void)render;

@end

