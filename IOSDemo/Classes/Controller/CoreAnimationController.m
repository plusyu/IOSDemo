//
//  CoreAnimationController.m
//  IOSDemo
//
//  Created by yuxiang on 14/12/2.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "CoreAnimationController.h"
#import "GreenLineLabel.h"
#import "MyProgress.h"
#import "ProgressView.h"



@interface CoreAnimationController()

- (IBAction)move:(id)sender;
@property (weak, nonatomic) IBOutlet UIButton *moveText;

- (IBAction)Rotation:(id)sender;
@property (weak, nonatomic) IBOutlet UIButton *rotationText;

@property (weak, nonatomic) IBOutlet UIButton *scaleText;

- (IBAction)scale:(id)sender;

@property (weak, nonatomic) IBOutlet UIButton *groupText;

- (IBAction)group:(id)sender;


- (IBAction)key:(id)sender;

@property (weak, nonatomic) IBOutlet UIButton *keyText;


@property (weak, nonatomic) IBOutlet UIButton *keyValueText;

- (IBAction)keyValue:(id)sender;

- (IBAction)drawStar:(id)sender;


@property (weak, nonatomic) IBOutlet UIButton *drawStarText;

@property(nonatomic)CGMutablePathRef starPath;


@property (weak, nonatomic) IBOutlet UIButton *greedText;


@property(strong,nonatomic)GreenLineLabel *lable ;
@property(nonatomic,strong)CALayer *maskLayer;

- (IBAction)greed:(id)sender;

- (IBAction)showProgress:(id)sender;

@property(nonatomic,assign)CGFloat progress;

//@property(nonatomic,strong)KYCircularProgress *circularProgress1;
@end

@implementation CoreAnimationController
@synthesize starPath;
- (void)viewDidLoad
{
    [super viewDidLoad];
//    starPath =CGPathCreateMutable();
//    CGPathMoveToPoint(starPath,NULL,240.0, 280.0);
//    NSLog(@"viewDidLoad");
    CGRect greenRect = CGRectMake(0, 180, self.view.frame.size.width, 200);
    self.lable = [[GreenLineLabel alloc]initWithFrame:greenRect];
    [self.lable setBackgroundColor:[UIColor blackColor]];
    [self.lable setTextColor:[UIColor blackColor]];
    [self.lable setText:@"我是测试的小酱油 "];
    [self.view addSubview: self.lable];
    
}

- (IBAction)move:(id)sender{
//    // 指定position属性
    CABasicAnimation *animation =
    [CABasicAnimation animationWithKeyPath:@"position"];
    animation.duration = 1; // 动画持续时间
    animation.repeatCount = 1; // 不重复
    animation.beginTime = CACurrentMediaTime()  ; // 2秒后执行
    animation.autoreverses = NO; // 结束后执行逆动画
    animation.removedOnCompletion = NO;
    animation.fillMode = kCAFillModeForwards;

    // 动画先加速后减速
//    *  动画的开始与结束的快慢,有五个预置分别为(下同):
//    *  kCAMediaTimingFunctionLinear            线性,即匀速
//    *  kCAMediaTimingFunctionEaseIn            先慢后快
//    *  kCAMediaTimingFunctionEaseOut           先快后慢
//    *  kCAMediaTimingFunctionEaseInEaseOut     先慢后快再慢
//    *  kCAMediaTimingFunctionDefault           实际效果是动画中间比较快.
    
    self.moveText.layer.anchorPoint = CGPointMake(0.5, 0.5);
    animation.timingFunction =
    [CAMediaTimingFunction functionWithName: kCAMediaTimingFunctionEaseInEaseOut ];
    // 设定动画起始帧和结束帧
    CGPoint ceter = self.moveText.center;
    animation.fromValue = [NSValue valueWithCGPoint:ceter]; // 起始点
    animation.toValue = [NSValue valueWithCGPoint:CGPointMake(300, ceter.y)]; // 终了点
    
    [self.moveText.layer addAnimation:animation forKey:@"move-layer"];
}
- (IBAction)Rotation:(id)sender {
    // 对Y轴进行旋转（指定Z轴的话，就和UIView的动画一样绕中心旋转）
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"transform.rotation.y"];
    
    // 设定动画选项
    animation.duration = 2.5; // 持续时间
    animation.repeatCount = 1; // 重复次数
    
    // 设定旋转角度
    animation.fromValue = [NSNumber numberWithFloat:0.0]; // 起始角度
    animation.toValue = [NSNumber numberWithFloat:2 * M_PI]; // 终止角度
    
    // 添加动画
    [self.rotationText.layer addAnimation:animation forKey:@"rotate-layer"];
}
- (IBAction)scale:(id)sender {
    /* 放大缩小 */
    
    // 设定为缩放
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"transform.scale"];
    
    // 动画选项设定
    animation.duration = 2.5; // 动画持续时间
    animation.repeatCount = 1; // 重复次数
    animation.autoreverses = YES; // 动画结束时执行逆动画
    
    // 缩放倍数
    animation.fromValue = [NSNumber numberWithFloat:1.0]; // 开始时的倍率
    animation.toValue = [NSNumber numberWithFloat:2.0]; // 结束时的倍率
    
    // 添加动画
    [self.scaleText.layer addAnimation:animation forKey:@"scale-layer"];
    
}
- (IBAction)group:(id)sender {
    
    /* 动画1（在X轴方向移动） */
    CABasicAnimation *animation1 =
    [CABasicAnimation animationWithKeyPath:@"transform.translation.x"];
    
    // 终点设定
    animation1.toValue = [NSNumber numberWithFloat:80];; // 終点
    
    
    /* 动画2（绕Z轴中心旋转） */
    CABasicAnimation *animation2 =
    [CABasicAnimation animationWithKeyPath:@"transform.rotation.z"];
    
    // 设定旋转角度
    animation2.fromValue = [NSNumber numberWithFloat:0.0]; // 开始时的角度
    animation2.toValue = [NSNumber numberWithFloat:4 * M_PI]; // 结束时的角度
    
    
    /* 动画组 */
    CAAnimationGroup *group = [CAAnimationGroup animation];
    
    // 动画选项设定
    group.duration = 3.0;
    group.repeatCount = 1;
    
    // 添加动画
    group.animations = [NSArray arrayWithObjects:animation1, animation2, nil];
    [self.groupText.layer addAnimation:group forKey:@"move-rotate-layer"];
}

- (IBAction)key:(id)sender {
    
    CGMutablePathRef path = CGPathCreateMutable();
    //将路径的起点定位到  （50  120）
    CGPathMoveToPoint(path,NULL,50.0,120.0);
    //下面5行添加5条直线的路径到path中
    CGPathAddLineToPoint(path, NULL, 60, 130);
    CGPathAddLineToPoint(path, NULL, 70, 140);
    CGPathAddLineToPoint(path, NULL, 80, 150);
    CGPathAddLineToPoint(path, NULL, 90, 160);
    CGPathAddLineToPoint(path, NULL, 100, 170);
    //下面四行添加四条曲线路径到path
    CGPathAddCurveToPoint(path,NULL,50.0,275.0,150.0,275.0,70.0,120.0);
    CGPathAddCurveToPoint(path,NULL,150.0,275.0,250.0,275.0,90.0,120.0);
    CGPathAddCurveToPoint(path,NULL,250.0,275.0,350.0,275.0,110.0,120.0);
    CGPathAddCurveToPoint(path,NULL,350.0,275.0,450.0,275.0,130.0,120.0);
    //以“position”为关键字 创建 实例
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    //设置path属性
    [animation setPath:path];
    [animation setDuration:3.0];
    //这句代码 表示 是否动画回到原位
     [animation setAutoreverses:YES];
    CFRelease(path);
    [self.keyText.layer addAnimation:animation forKey:NULL];
}
- (IBAction)keyValue:(id)sender {
    
    
    CGPoint center =  self.keyValueText.center;
    
    CGPoint p1=CGPointMake(200, center.y);
    CGPoint p2=CGPointMake(250, center.y);
    CGPoint p3=CGPointMake(300, center.y);
//    CGPoint p4=CGPointMake(150, center.y);
//    CGPoint p5=CGPointMake(340, center.y);
  
    NSArray *values=[NSArray arrayWithObjects:
                     [NSValue valueWithCGPoint:p1],
                     [NSValue valueWithCGPoint:p2],
                     [NSValue valueWithCGPoint:p3],
                      nil];
    CAKeyframeAnimation
    *animation = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    
    
    [animation setCalculationMode:kCAAnimationLinear]; [animation setKeyTimes:
                                                        [NSArray arrayWithObjects:
                                                         [NSNumber numberWithFloat:0.0],
                                                         [NSNumber numberWithFloat:0.4],
                                                         [NSNumber numberWithFloat:1.0],
                                                         nil]];
    
    [animation setValues:values];
    [animation setDuration:10.0];
    [animation setAutoreverses:NO];
    
    
    [self.keyValueText.layer addAnimation:animation forKey:@"position"];
}

- (IBAction)drawStar:(id)sender {
    
    //创建五角星的路径
    CGMutablePathRef path = CGPathCreateMutable();
    CGPathMoveToPoint   (path,NULL,  240.0, 280.0);
    CGPathAddLineToPoint(path, NULL, 181.0, 99.0);
    CGPathAddLineToPoint(path, NULL, 335.0, 210.0);
    CGPathAddLineToPoint(path, NULL, 144.0, 210.0);
    CGPathAddLineToPoint(path, NULL, 298.0, 99.0);
    CGPathCloseSubpath(path);
    
    //创建关键帧动画实例
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    //关键帧动画定义为10秒，一共6帧，每一帧是10/（6-1）等与2秒
    [animation setDuration:10.0f];
    [animation setDelegate:self];
    [animation setPath:path];
    
    
    [self.drawStarText.layer addAnimation:animation forKey:NULL];
    
    //在每一帧结束的时候  定时器进入其方法
    [NSTimer scheduledTimerWithTimeInterval:2.0 target:self
                                   selector:@selector(legOne:) userInfo:nil
                                    repeats:NO];
    [NSTimer scheduledTimerWithTimeInterval:4.0
                                     target:self selector:@selector(legTwo:) userInfo:nil
                                    repeats:NO];
    [NSTimer scheduledTimerWithTimeInterval:6.0 target:self
                                   selector:@selector(legThree:) userInfo:nil repeats:NO];
    
    [NSTimer scheduledTimerWithTimeInterval:8.0 target:self selector:@selector(legFour:) userInfo:nil
                                    repeats:NO];
    [NSTimer scheduledTimerWithTimeInterval:10.0 target:self selector:@selector(legFive:) userInfo:nil
                                    repeats:NO];
    // Tell the root layer to call drawLayer
    
    //不设定这个Delegate  调用setNeedDisplay的时候  会不进入 drawLayer方法
//    [self.view.layer setDelegate:self];
}

- (void)legOne:(id)sender {
    CGPathAddLineToPoint(starPath, NULL, 181.0, 99.0);
    [self.view.layer setNeedsDisplay];
}
- (void)legTwo:(id)sender { CGPathAddLineToPoint(starPath, NULL, 335.0, 210.0);
    [self.view.layer setNeedsDisplay];
}
- (void)legThree:(id)sender { CGPathAddLineToPoint(starPath, NULL, 144.0, 210.0);
    [self.view.layer setNeedsDisplay];
}
- (void)legFour:(id)sender { CGPathAddLineToPoint(starPath, NULL, 298.0, 99.0);
    [self.view.layer setNeedsDisplay];
}
- (void)legFive:(id)sender { CGPathCloseSubpath(starPath);
    [self.view.layer setNeedsDisplay];
}

//- (void)drawLayer:(CALayer *)layer inContext:(CGContextRef)context {
//        
//        CGColorRef white =[UIColor whiteColor].CGColor;
//            CGContextSetStrokeColorWithColor(context, white);
//        //CFRelease(white);
//        CGContextBeginPath(context);
//        CGContextAddPath(context, starPath);
//        CGContextSetLineWidth(context, 3.0);
//        CGContextStrokePath(context);
//        NSLog(@"im in drawLayer");
//}
- (IBAction)greed:(id)sender {
    self.lable.backgroundColor = [UIColor greenColor];

    CGPoint p1 = CGPointMake(0, 0);
    CGPoint p2 = CGPointMake(250,0);
    CGPoint p3 = CGPointMake(400, 0);

    NSMutableArray *keyTimes = [[NSMutableArray alloc]init];
    [keyTimes addObject:[NSNumber numberWithFloat:0.0]];
    [keyTimes addObject:[NSNumber numberWithFloat:0.6]];
    [keyTimes addObject:[NSNumber numberWithFloat:1.0]];
    
    NSMutableArray *values = [[NSMutableArray alloc]init];
    [values addObject:[NSValue valueWithCGPoint:p1]];
    [values addObject:[NSValue valueWithCGPoint:p2]];
    [values addObject:[NSValue valueWithCGPoint:p3]];
    
    [self.lable startAnimation:keyTimes values:values duration:10.0];
    

}

- (IBAction)showProgress:(id)sender {
    ProgressView *pregress = [[ProgressView alloc]initWithFrame:
                            CGRectMake(0, 80, self.view.frame.size.width, self.view.frame.size.height)];
    pregress.backgroundColor = [UIColor clearColor];

    [self.view addSubview:pregress];
 
}

/*
-(void)news
{
    self.circularProgress1  = [[KYCircularProgress alloc ]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height/2)];
    self.circularProgress1.path = [UIBezierPath bezierPathWithArcCenter:CGPointMake(160, 200) radius:self.circularProgress1.frame.size.width/3.0 startAngle:M_PI * 0.72 endAngle:M_PI * 2.28 clockwise:true];
    self.circularProgress1.lineWidth = 1.0;
    UILabel *textLabel = [[UILabel alloc]initWithFrame:CGRectMake(self.circularProgress1.frame.origin.x + 120.0, 170.0, 80.0, 32.0)];
    textLabel.textColor = [UIColor greenColor];
    textLabel.alpha = 0.3;
    [self.view addSubview:textLabel];
    
    [self.circularProgress1 progressChangedClosure:^(double prog, KYCircularProgress * progress) {
        textLabel.text = [@(prog) stringValue];
    }];
    
    [self.view addSubview:self.circularProgress1];
    
    [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(updateProgress) userInfo:nil repeats:true];
}


-(void)updateProgress
{
    self.progress = self.progress + 1;
    CGFloat normalizedProgress = self.progress / 255.0;
    self.circularProgress1.progress = normalizedProgress;
}
 */


@end
