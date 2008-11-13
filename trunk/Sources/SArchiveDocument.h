/*
 *  SArchiveDocument.h
 *  SArchiveKit
 *
 *  Created by Shadow Team.
 *  Copyright (c) 2007 Shadow Lab. All rights reserved.
 */

WB_CLASS_EXPORT
@interface SArchiveDocument : NSObject {
  @private
  void *sa_ptr;
  void *sa_arch;
  NSString *sa_name;
}

- (NSString *)name;
- (void)setName:(NSString *)aName;

  /* Properties */
- (NSString *)valueForProperty:(NSString *)prop;
- (void)setValue:(NSString *)value forProperty:(NSString *)prop;

  /* Attributes */
- (NSString *)valueForAttribute:(NSString *)attr property:(NSString *)prop;
- (void)setValue:(NSString *)value forAttribute:(NSString *)attr property:(NSString *)property;

@end
