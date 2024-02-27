//  ────────────────────────────────────────────────────────────
//                     ╔╗  ╔╗ ╔══╗      ╔════╗
//                     ║╚╗╔╝║ ╚╣╠╝      ║╔╗╔╗║
//                     ╚╗╚╝╔╝  ║║  ╔══╗ ╚╝║║╚╝
//                      ╚╗╔╝   ║║  ║╔╗║   ║║
//                       ║║   ╔╣╠╗ ║╚╝║   ║║
//                       ╚╝   ╚══╝ ╚══╝   ╚╝
//    ╔╗╔═╗                    ╔╗                     ╔╗
//    ║║║╔╝                   ╔╝╚╗                    ║║
//    ║╚╝╝  ╔══╗ ╔══╗ ╔══╗  ╔╗╚╗╔╝  ╔══╗ ╔╗ ╔╗╔╗ ╔══╗ ║║  ╔══╗
//    ║╔╗║  ║║═╣ ║║═╣ ║╔╗║  ╠╣ ║║   ║ ═╣ ╠╣ ║╚╝║ ║╔╗║ ║║  ║║═╣
//    ║║║╚╗ ║║═╣ ║║═╣ ║╚╝║  ║║ ║╚╗  ╠═ ║ ║║ ║║║║ ║╚╝║ ║╚╗ ║║═╣
//    ╚╝╚═╝ ╚══╝ ╚══╝ ║╔═╝  ╚╝ ╚═╝  ╚══╝ ╚╝ ╚╩╩╝ ║╔═╝ ╚═╝ ╚══╝
//                    ║║                         ║║
//                    ╚╝                         ╚╝
//
//    Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>
//  ────────────────────────────────────────────────────────────>

#include <KSWiFi.h>



#include <QWindow>
#include <qpa/qplatformnativeinterface.h>
#include <QDebug>

#import <UIKit/UIKit.h>
#import <QGuiApplication>
#import <QQuickWindow>

#import <Foundation/Foundation.h>
#import <SystemConfiguration/CaptiveNetwork.h>
#import "KSiOSHelpers.h"


//-----------------------------------------------------------------------------
void
iosUiPrepare() {

}

//-----------------------------------------------------------------------------
int
iosUiStatusBarHeight() {
    static int h = -1;
    static const int kDefaultH = 44;
    
    if (h >= 0) {
        return h;
    }
    
    auto window = QGuiApplication::focusWindow();
    if (!window) {
        return kDefaultH;
    }
    UIView *view = static_cast<UIView*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", window));
    if (!view) {
        return kDefaultH;
    }
   	 
    UIWindowScene *scene = [[view window] windowScene];
    if (!scene) {
        return kDefaultH;
    }
    
    h = [scene statusBarManager].statusBarFrame.size.height;
    
    return h;
}

//-----------------------------------------------------------------------------
void
iosUiShareDialog(QString url) {
    NSMutableArray *sharingItems = [NSMutableArray new];
    
    NSString *filePath = url.toNSString();
    [sharingItems addObject:[NSURL fileURLWithPath:filePath]];
    auto window = QGuiApplication::focusWindow();
    if (!window) {
        return;
    }
    UIView *view = static_cast<UIView*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", window));
    if (!view) {
        return;
    }

    UIViewController* qtController = [[view window] rootViewController];
    if (!qtController) {
        return;
    }

    // [controller setNeedsStatusBarAppearanceUpdate];
    
    UIActivityViewController *activityController = [[UIActivityViewController alloc] initWithActivityItems:sharingItems applicationActivities:nil];
    if ( [activityController respondsToSelector:@selector(popoverPresentationController)] ) {
        activityController.popoverPresentationController.sourceView = qtController.view;
    }
    [qtController presentViewController:activityController animated:YES completion:nil];
}

//-----------------------------------------------------------------------------
