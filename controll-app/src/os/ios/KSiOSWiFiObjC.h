#import <CoreLocation/CoreLocation.h>
#import <NetworkExtension/NetworkExtension.h>

@interface KSiOSWiFiObjC : NSObject<CLLocationManagerDelegate>

@property (nonatomic, strong) CLLocationManager *locationManager;

- (NSString *)ssid;
- (NSString *)routeIp;

@end

