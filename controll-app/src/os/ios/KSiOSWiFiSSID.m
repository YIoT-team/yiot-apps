#import "KSiOSWiFiSSID.h"

@implementation KSiOSWiFiSSID {
    NSLock *_mutex;
    NSString *_ssid;
}

- (void)startLocationManager {
    if (self.locationManager) {
        return;
    }
    
    _mutex = [NSLock new];
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
    self.locationManager.desiredAccuracy = kCLLocationAccuracyKilometer;
    [self.locationManager requestWhenInUseAuthorization];
    [self.locationManager startUpdatingLocation];
}

- (NSString *)ssid {
    [self startLocationManager];
    NSString *res;
    [_mutex lock];
    res = [_ssid copy];
    [_mutex unlock];
    
    return res;
}

#pragma mark - CLLocationManagerDelegate

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations {
    if (@available(iOS 14.0, *)) {
        [NEHotspotNetwork fetchCurrentWithCompletionHandler:^(NEHotspotNetwork * _Nullable currentNetwork) {
            [_mutex lock];
            _ssid = [currentNetwork SSID];
            [_mutex unlock];
        }];
    }
}

- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status {
    // Detect the CLAuthorizationStatus and enable the capture of associated SSID
    if (status == kCLAuthorizationStatusAuthorizedAlways ||
        status == kCLAuthorizationStatusAuthorizedWhenInUse) {
        // Handle authorization status
        NSLog(@">>> authorization OK");
        [self.locationManager startUpdatingLocation];
    } else {
        NSLog(@">>> authorization ERROR : %d", status);
        [self.locationManager requestWhenInUseAuthorization];
    }
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error {
    NSLog(@">>> didFailWithError: %@ ", [error localizedDescription]);
}

@end
