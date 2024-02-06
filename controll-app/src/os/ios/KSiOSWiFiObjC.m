#import "KSiOSWiFiObjC.h"


#include <ifaddrs.h>
#include <arpa/inet.h>

@implementation KSiOSWiFiObjC {
    NSLock *_mutex;
    NSString *_ssid;
}

- (NSString *)routeIp {

  NSString *address = @"error";
  struct ifaddrs *interfaces = NULL;
  struct ifaddrs *temp_addr = NULL;
  int success = 0;

  // retrieve the current interfaces - returns 0 on success
  success = getifaddrs(&interfaces);
  if (success == 0)
  {
    // Loop through linked list of interfaces
    temp_addr = interfaces;
    while(temp_addr != NULL)
    {
      if(temp_addr->ifa_addr->sa_family == AF_INET)
      {
        // Check if interface is en0 which is the wifi connection on the iPhone
        if([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"])
        {
          // Get NSString from C String //ifa_addr
          address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_dstaddr)->sin_addr)];
        }
      }

      temp_addr = temp_addr->ifa_next;
    }
  }

  // Free memory
  freeifaddrs(interfaces);

  return address;
}

- (void)startLocationManager {
    if (self.locationManager) {
        return;
    }
    
    _mutex = [NSLock new];
    _ssid = @"";
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
            _ssid = [[currentNetwork SSID] copy];
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
