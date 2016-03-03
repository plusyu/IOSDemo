//
//  MyUIViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/13.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "MyUIViewController.h"
#import "HideAlertView.h"
#import "LoginTask.h"
#import "MBProgressHUD.h"
#import "CustomIOS7AlertView.h"
#import "LoginTask.h"
#import "AchorInfo.h"
#import "V6ResponseError.h"
#import "AlertUtil.h"
#import "AFNetworking.h"
#import "Sculptor.h"
#import "AchorInfo.h"
#import <SDWebImage/UIImageView+WebCache.h>
#import "V6UploadTask.h"
#import "V6DownLoadTask.h"
#import "TouchViewController.h"

@interface MyUIViewController ()
{
    NSURLSessionDownloadTask *getImageTask;
    NSURLSession *session;
    NSURLRequest *request;
    V6UploadTask *uploadTask;
    V6DownLoadTask *downLoadTask;
}

@property (strong, nonatomic) TouchViewController *touchController;

- (IBAction)showKeyBorad:(id)sender;
@property (weak, nonatomic) IBOutlet UITextField *textView;
@property(nonatomic,assign)BOOL isDownLoad;


- (IBAction)showAlertView:(id)sender;
- (IBAction)testNetWork:(id)sender;
- (IBAction)showProgress:(id)sender;
- (IBAction)didTapLogin:(id)sender;
- (IBAction)didTapUpload:(id)sender;
- (IBAction)didTapDownload:(id)sender;

- (IBAction)didStop:(id)sender;

@property (weak, nonatomic) IBOutlet UIButton *stopText;

@property (weak, nonatomic) IBOutlet UIImageView *imageView;


@end

@implementation MyUIViewController

@synthesize textView = _textView;

- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationItem.title = @"UI";

    // Observe keyboard hide and show notifications to resize the text view appropriately.
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
    
    downLoadTask = [[V6DownLoadTask alloc]init];
    uploadTask =[[V6UploadTask alloc]init];
    
}

- (void)viewWillAppear:(BOOL)animated {
    
    // Make the keyboard appear when the application launches.
//    [super viewWillAppear:animated];
//    [self.textView becomeFirstResponder];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

#pragma mark - text delegate


-(BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    /*
     You can create the accessory view programmatically (in code), in the same nib file as the view controller's main view, or from a separate nib file. This example illustrates the latter; it means the accessory view is loaded lazily -- only if it is required.
     */
//    
//    if (self.textView.inputAccessoryView == nil) {
//        [[NSBundle mainBundle] loadNibNamed:@"AccessoryView" owner:self options:nil];
//        // Loading the AccessoryView nib file sets the accessoryView outlet.
//        self.textView.inputAccessoryView = accessoryView;
//        // After setting the accessory view for the text view, we no longer need a reference to the accessory view.
//        self.accessoryView = nil;
//    }
    
    return YES;
}

- (BOOL)textViewShouldEndEditing:(UITextView *)aTextView {
    [aTextView resignFirstResponder];
    return YES;
}


#pragma mark - 键盘
#pragma mark Responding to keyboard events

- (void)keyboardWillShow:(NSNotification *)notification {
    
    /*
     Reduce the size of the text view so that it's not obscured by the keyboard.
     Animate the resize so that it's in sync with the appearance of the keyboard.
     */
    
    NSDictionary *userInfo = [notification userInfo];
    
    // Get the origin of the keyboard when it's displayed.
    NSValue* aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
    
    // Get the top of the keyboard as the y coordinate of its origin in self's view's coordinate system. The bottom of the text view's frame should align with the top of the keyboard's final position.
    CGRect keyboardRect = [aValue CGRectValue];
    keyboardRect = [self.view convertRect:keyboardRect fromView:nil];
    
    CGFloat keyboardTop = keyboardRect.origin.y;
    CGRect newTextViewFrame = self.view.bounds;
    newTextViewFrame.size.height = keyboardTop - self.view.bounds.origin.y;
    
    // Get the duration of the animation.
    NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
    NSTimeInterval animationDuration;
//    [animationDurationValue getValue:&amp;animationDuration];

    
    // Animate the resize of the text view's frame in sync with the keyboard's appearance.
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:animationDuration];
    
    self.textView.frame = newTextViewFrame;
    
    [UIView commitAnimations];
}

- (void)keyboardWillHide:(NSNotification *)notification {
    
    NSDictionary* userInfo = [notification userInfo];
    
    /*
     Restore the size of the text view (fill self's view).
     Animate the resize so that it's in sync with the disappearance of the keyboard.
     */
    NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
    NSTimeInterval animationDuration;
//    [animationDurationValue getValue:&amp;animationDuration];
    
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:animationDuration];
    
    self.textView.frame = self.view.bounds;

    
    [UIView commitAnimations];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (IBAction)showKeyBorad:(id)sender {
}
- (IBAction)showAlertView:(id)sender {
    HideAlertView *alertView = [[HideAlertView alloc] initWithTitle:@"正在加载..." message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
    [alertView show];
    [self performSelector:@selector(loadAlertView:) withObject:alertView afterDelay:2.0f];
}

- (IBAction)testNetWork:(id)sender {
//    LoginTask *task = [[LoginTask alloc]init];
//    [task execute:@"" onSuccess:^{
//        NSLog(@"onSuccess");
//    }exception:^{
//        NSLog(@"OnExcepton");
//    }];
}

- (IBAction)showProgress:(id)sender {
    
//    MBProgressHUD *hud = [[MBProgressHUD alloc]initWithView:self.view];
//    [self.view addSubview:hud];
//    hud.labelText = @"操作成功";
//    hud.mode = MBProgressHUDModeDeterminateHorizontalBar;
//    [hud show:YES];
    
//    CustomIOS7AlertView *alertView = [[CustomIOS7AlertView alloc]init];
//    
//    [alertView setContainerView:nil];
//
//    // Modify the parameters
//    [alertView setButtonTitles:nil];
//    
//    // You may use a Block, rather than a delegate.
//    [alertView setOnButtonTouchUpInside:^(CustomIOS7AlertView *alertView, int buttonIndex) {
//
//    }];
//    
//    [alertView setUseMotionEffects:true];
//    [alertView show];
    
    
    UIAlertView *remoteAlertView =  [[UIAlertView alloc] initWithTitle:@"提示" message:@"等待" delegate:self cancelButtonTitle:nil otherButtonTitles:nil, nil ];
    UIActivityIndicatorView *aiView = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(125.0, 80.0, 30.0, 30.0)];
    aiView.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
    //check if os version is 7 or above. ios7.0及以上UIAlertView弃用了addSubview方法
    
    [remoteAlertView setValue:aiView forKey:@"accessoryView"];
    
    [remoteAlertView show];
    [aiView startAnimating];
    
}

- (IBAction)didTapLogin:(id)sender {
    LoginTask *task = [[LoginTask alloc]init];
    [task execute:@"" onSuccess:^(id obj) {
         NSLog(@"成功:%@",obj);
//        AchorInfo *info =(AchorInfo *)obj;
//      NSString * flag =   [info flag];
        
    } exception:^(id obj) {
        NSLog(@"失败:%@",obj);
        [AlertUtil handleExcption:obj];
        
    }];
    
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    manager.responseSerializer = [SCLMantleResponseSerializer serializerForModelClass:[AchorInfo class]];
    
    
    NSDictionary *params = @{@"padapi": @"coop-mobile-searchAnchor.php",
                             @"rate": @"1",
                             @"key": @"3098",
                             @"p": @"1",
                             @"size": @"1",
                             @"t": @"r",
                             };
    
    
    [manager GET:@"http://v.6.cn/coop/mobile/index.php" parameters:params success:^(AFHTTPRequestOperation *operation, AchorInfo *user) {
        // GHUser is an MTLModel subclass
        NSLog(@"User model is %@", user.flag);
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"Error fetching user: %@", error);
    }];
    
    
}
NSString * const imageUrl =@"http://vr0.6.cn/mobile/test/nba.gif";
NSString * const zipUrl = @"http://vr0.6.cn/mobile/test/123.mp3";
NSString * const mp3Url = @"http://vr0.6.cn/mobile/test/5.mp3";
- (IBAction)didTapUpload:(id)sender {
    

//    [downLoadTask execute:imageUrl view:nil];

//    [uploadTask execute:imageUrl view:self.imageView];
    
    
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];

    
    session  = [NSURLSession sessionWithConfiguration:sessionConfig
                                  delegate:self
                             delegateQueue:nil];
    
    request = [NSURLRequest requestWithURL:[NSURL URLWithString:imageUrl]];
    getImageTask = [session downloadTaskWithRequest:request];
    [getImageTask resume];
    getImageTask = [session downloadTaskWithRequest:request completionHandler:^(NSURL *location, NSURLResponse *response, NSError *error) {
         UIImage *downloadedImage =[UIImage imageWithData:[NSData dataWithContentsOfURL:location]];
    }];
    [getImageTask resume];
    
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
    
    
    //刷新进度条的delegate方法，同样的，获取数据，调用主线程刷新UI
    double currentProgress = totalBytesWritten/(double)totalBytesExpectedToWrite;
    NSLog(@"下载:%f",currentProgress);
    
}


-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
    //下载成功后，文件是保存在一个临时目录的，需要开发者自己考到放置该文件的目录
    NSLog(@"Download success for URL: %@",location.description);
    NSURL *destination = [self createDirectoryForDownloadItemFromURL:location];
    BOOL success = [self copyTempFileAtURL:location toDestination:destination];
    
    if(success){
        //        文件保存成功后，使用GCD调用主线程把图片文件显示在UIImageView中
        dispatch_async(dispatch_get_main_queue(), ^{
            UIImage *image = [UIImage imageWithContentsOfFile:[destination path]];
            self.imageView.image = image;
            self.imageView.contentMode = UIViewContentModeScaleAspectFit;
            self.imageView.hidden = NO;
        });
    }else{
        NSLog(@"Meet error when copy file");
    }
    getImageTask = nil;
}


//创建文件本地保存目录
-(NSURL *)createDirectoryForDownloadItemFromURL:(NSURL *)location
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *urls = [fileManager URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask];
    NSURL *documentsDirectory = urls[0];
    NSLog(@"location lastPathComponent]:%@",[location lastPathComponent]);
    return [documentsDirectory URLByAppendingPathComponent:[location lastPathComponent]];
}
//把文件拷贝到指定路径
-(BOOL) copyTempFileAtURL:(NSURL *)location toDestination:(NSURL *)destination
{
    
    NSError *error;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager removeItemAtURL:destination error:NULL];
    [fileManager copyItemAtURL:location toURL:destination error:&error];
    if (error == nil) {
        return true;
    }else{
        NSLog(@"%@",error);
        return false;
    }
}



- (IBAction)didTapDownload:(id)sender {
    [downLoadTask execute:mp3Url view:nil];
}

- (IBAction)didStop:(id)sender {

    [uploadTask cancel];
    [downLoadTask cancel];
    [uploadTask cancel];
    
    if (self.isDownLoad) {
        NSLog(@"stop");
        self.isDownLoad = NO;
        //判断是否又已下载数据，有的话就断点续传，没有就完全重新下载
        [self.stopText setTitle:@"恢复下载中" forState:UIControlStateNormal];
        if (self.partialData) {
            getImageTask = [session downloadTaskWithResumeData:self.partialData];
        }else{
            getImageTask = [session downloadTaskWithRequest:request];
        }
         [getImageTask resume];
        
    }else{
        NSLog(@"resume");
        self.isDownLoad = YES;

        [self.stopText setTitle:@"暂停中" forState:UIControlStateNormal];
        
        [getImageTask cancelByProducingResumeData:^(NSData *resumeData) {
            self.partialData = resumeData;
            getImageTask = nil;
        }];
    }
}


#pragma mark - --------------------------对话框--------------------------

#pragma mark 显示对话框
- (void)loadAlertView:(HideAlertView *) loadingAlertView {
    [loadingAlertView loaded];
    [self performSelector:@selector(removeAlertView:) withObject:loadingAlertView afterDelay:0.0f];
}
#pragma mark 隐藏对话框
- (void)removeAlertView:(HideAlertView *) loadingAlertView {
    [loadingAlertView dismissWithClickedButtonIndex:0 animated:YES];
}

- (IBAction)showTouchController:(id)sender {
    self.touchController  = [[TouchViewController alloc]init];
    [self.navigationController pushViewController:self.touchController animated:YES];
}




@end
