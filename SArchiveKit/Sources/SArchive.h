/*
 *  SArchive.h
 *  SArchiveKit
 *
 *  Created by Jean-Daniel Dupas.
 *  Copyright (c) 2008 Jean-Daniel Dupas. All rights reserved.
 */
/*! \file SArchive.h
 
 */
#include <Security/Security.h>
#import <SArchiveKit/SABase.h>

/* setting owner/group behavior */
SA_EXPORT NSString * const SArchiveOptionOwnershipBehaviourKey;
/* set owner/group based on names */
SA_EXPORT NSString * const SArchiveOptionOwnershipSymbolic;
/* set owner/group based on uid/gid */
SA_EXPORT NSString * const SArchiveOptionOwnershipNumeric;

/* Save setuid/setgid bits */
SA_EXPORT NSString * const SArchiveOptionSaveSUID;

/* set the toc checksum algorithm */
SA_EXPORT NSString * const SArchiveOptionTocCheckSumKey;
/* set the file checksum algorithm */
SA_EXPORT NSString * const SArchiveOptionFileCheckSumKey;

SA_EXPORT NSString * const SArchiveOptionCheckSumNone;
SA_EXPORT NSString * const SArchiveOptionCheckSHA1;
SA_EXPORT NSString * const SArchiveOptionCheckMD5;

/* set the file compression type */
SA_EXPORT NSString * const SArchiveOptionCompressionKey;

SA_EXPORT NSString * const SArchiveOptionCompressionGZip;
SA_EXPORT NSString * const SArchiveOptionCompressionBZip;
SA_EXPORT NSString * const SArchiveOptionCompressionLZMA;

SA_EXPORT NSString * const SArchiveOptionIncludedProperty;
SA_EXPORT NSString * const SArchiveOptionExcludedProperty;

@class SArchiveSignature;
@class SArchiveFile, SArchiveDocument;
SA_CLASS_EXPORT
@interface SArchive : NSObject <NSFastEnumeration> {
@private
  void *sa_arch; // xar_t
  NSString *sa_path;
  NSMapTable *sa_files;
  NSMutableArray *sa_roots;
  NSMutableArray *sa_signatures;
  NSMutableDictionary *sa_documents;
  
  /* extract context */
  id sa_delegate;
  int32_t sa_extract; /* atomic lock */
  struct _sa_arFlags {
    unsigned int ok:1;
    // volatile to prevent aggressive optimization.
    volatile unsigned int cancel:1;
    unsigned int reserved:30;
  } sa_arFlags;
}

- (id)initWithArchiveAtPath:(NSString *)path;
- (id)initWithArchiveAtPath:(NSString *)path write:(BOOL)write;

- (id)initWithArchiveAtURL:(NSURL *)anURL;
- (id)initWithArchiveAtURL:(NSURL *)anURL write:(BOOL)write;

- (NSString *)path;

- (void)close;

/* size after extraction */
- (UInt64)size;
- (NSUInteger)fileCount;

/*!
 @method
 @discussion Files are not sorted in any way. 
 						You should use Obj-C 2 "fast enumeration" instead of this when possible.
 */
- (NSEnumerator *)fileEnumerator;

/* Returns root files */
- (NSArray *)files;
- (SArchiveFile *)fileWithName:(NSString *)name;

- (void)includeProperty:(NSString *)name;
- (void)excludeProperty:(NSString *)name;

- (NSString *)valueForOption:(NSString *)key;
- (void)setValue:(NSString *)opt forOption:(NSString *)key;
- (BOOL)boolValueForOption:(NSString *)key;
- (void)setBoolValue:(BOOL)value forOption:(NSString *)key;

- (SArchiveFile *)addFile:(NSString *)path;
- (SArchiveFile *)addFile:(NSString *)path name:(NSString *)name parent:(SArchiveFile *)parent;

- (SArchiveFile *)addFile:(NSString *)name data:(NSData *)data parent:(SArchiveFile *)parent;
- (SArchiveFile *)addFolder:(NSString *)name properties:(NSDictionary *)props parent:(SArchiveFile *)parent;

- (SArchiveFile *)addFileWrapper:(NSFileWrapper *)aWrapper parent:(SArchiveFile *)parent;

#pragma mark Sub Document
- (SArchiveDocument *)documentWithName:(NSString *)name;
- (SArchiveDocument *)addDocumentWithName:(NSString *)name;

#pragma mark Signatures
- (NSArray *)signatures;
- (SArchiveSignature *)addSignature:(SecIdentityRef)identity;
- (SArchiveSignature *)addSignature:(SecIdentityRef)identity includeCertificate:(BOOL)include;

/*!
 @method
 @param handler should implements 
 <code>- (void)archive:(SArchive *)manager willProcessFile:(SArchiveFile *)path</code> and
 <code>- (BOOL)archive:(SArchive *)manager shouldProceedAfterError:(NSError *)anError severity:(SArchiveErrorLevel)severity</code>.
 keys: @"ArchiveFile".
 */
- (BOOL)extractToPath:(NSString *)path handler:(id)handler;

/* cancel background extraction */
- (void)cancel;

@end

SA_EXPORT NSString * const SArchiveErrorDomain;

enum {
	kSArchiveLevelDebug    = 1,
	kSArchiveLevelInfo     = 2,
	kSArchiveLevelNormal   = 3,
	kSArchiveLevelWarning  = 4,
	kSArchiveLevelNonFatal = 5,
	kSArchiveLevelFatal    = 6,
};
typedef NSInteger SArchiveErrorLevel;

/* Extraction */
@interface NSObject (SArchiveHandler)

- (BOOL)archive:(SArchive *)manager shouldProcessFile:(SArchiveFile *)file;
- (void)archive:(SArchive *)manager willProcessFile:(SArchiveFile *)file;
//- (void)archive:(SArchive *)manager extractingFile:(SArchiveFile *)file progress:(CGFloat)progress;
- (void)archive:(SArchive *)manager didProcessFile:(SArchiveFile *)file path:(NSString *)filePath;

- (void)archive:(SArchive *)manager didExtract:(BOOL)result path:(NSString *)path;

- (BOOL)archive:(SArchive *)manager shouldProceedAfterError:(NSError *)anError severity:(SArchiveErrorLevel)severity;

@end

