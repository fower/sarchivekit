/*
 *  SArchiveSignature.h
 *  SArchiveKit
 *
 *  Created by Shadow Team.
 *  Copyright (c) 2007 Shadow Lab. All rights reserved.
 */

#import <Security/Security.h>

WB_EXPORT
NSString * const kSArchiveSignatureSHA1WithRSA;

WB_CLASS_EXPORT
@interface SArchiveSignature : NSObject {
  @private
  void *sa_ptr;
  void *sa_arch;
  
  SecIdentityRef sa_identity;
}

- (SecIdentityRef)identity;

- (NSArray *)certificates;
- (OSStatus)addCertificate:(SecCertificateRef)cert;

- (BOOL)verify:(SecCertificateRef)certificate;
- (OSStatus)getDigest:(NSData **)digest signature:(NSData **)signdata;

@end
