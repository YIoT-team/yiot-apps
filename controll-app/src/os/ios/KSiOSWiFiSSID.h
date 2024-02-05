#import <CoreLocation/CoreLocation.h>
#import <NetworkExtension/NetworkExtension.h>

@interface KSiOSWiFiSSID : NSObject<CLLocationManagerDelegate>

@property (nonatomic, strong) CLLocationManager *locationManager;

- (NSString *)ssid;

@end

