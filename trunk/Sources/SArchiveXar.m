/*
 *  SArchiveXar.m
 *  SArchiveKit
 *
 *  Created by Shadow Team.
 *  Copyright (c) 2007 Shadow Lab. All rights reserved.
 */

/* MUST be before other imports */
#import <SArchiveKit/SArchiveFile.h>
#import <SArchiveKit/SArchiveDocument.h>
#import <SArchiveKit/SArchiveSignature.h>

#import "SArchiveXar.h"

/* Nasty forward declaration from filetree.h */
struct __xar_file_t {
	const struct __xar_prop_t *props;
	const struct __xar_attr_t *attrs;
	const char *prefix;
	const char *ns;
	const char *fspath;
	char parent_extracted;
	const struct __xar_file_t *parent;
	const struct __xar_file_t *children;
	const struct __xar_file_t *next;
	void *eas; //xar_ea_t eas;
	uint64_t nexteaid;
};

/* Properties */
typedef int32_t(*SArchiveXarPropertyGetter)(const void *ptr, const char *name, const char **value);
typedef int32_t(*SArchiveXarPropertySetter)(const void *ptr, const char *name, const char *value);

/* Attributes */
typedef const char *(*SArchiveXarAttributeGetter)(const void *ptr, const char *property, const char *name);
typedef int32_t(*SArchiveXarAttributeSetter)(const void *ptr, const char *property, const char *name, const char *value);

xar_file_t xar_file_get_parent(xar_file_t file) {
  return file->parent;
}

WB_INLINE
NSString *__SArchiveXarPropertyGet(const void *ptr, NSString *property, SArchiveXarPropertyGetter fct) {
  NSString *str = nil;
  if (ptr && property) {
    const char *value = NULL;
    if (0 == fct(ptr, [property UTF8String], &value) && value) {
      str = [NSString stringWithUTF8String:value];
    }
  }
  return str;
}

WB_INLINE
int32_t __SArchiveXarPropertySet(const void *ptr, NSString *property, NSString *value, SArchiveXarPropertySetter fct) {
  if (ptr && property) {
    return fct(ptr, [property UTF8String], [value UTF8String]);
  }
  return -1;
}

WB_INLINE
NSString *__SArchiveXarAttributeGet(const void *ptr, NSString *property, NSString *attribute, SArchiveXarAttributeGetter fct) {
  NSString *str = nil;
  if (ptr && property && attribute) {
    const char *value = fct(ptr, [property UTF8String], [attribute UTF8String]);
    if (value) {
      str = [NSString stringWithUTF8String:value];
    }
  }
  return str;
}

WB_INLINE
int32_t __SArchiveXarAttributeSet(const void *ptr, NSString *property, NSString *attribute, NSString *value, SArchiveXarAttributeSetter fct) {
  if (ptr && property && attribute) {
    return fct(ptr, [property UTF8String], [attribute UTF8String], [value UTF8String]);
  }
  return -1;
}

#pragma mark Xar File
NSString *SArchiveXarFileGetProperty(xar_file_t file, NSString *property) {
  return __SArchiveXarPropertyGet((void *)file, property, (SArchiveXarPropertyGetter)xar_prop_get);
}

void SArchiveXarFileSetProperty(xar_file_t file, NSString *property, NSString *value) {
  __SArchiveXarPropertySet((void *)file, property, value, (SArchiveXarPropertySetter)xar_prop_set);
}

NSString *SArchiveXarFileGetAttribute(xar_file_t file, NSString *property, NSString *attribute) {
  return __SArchiveXarAttributeGet((void *)file, property, attribute, (SArchiveXarAttributeGetter)xar_attr_get);
}

void SArchiveXarFileSetAttribute(xar_file_t file, NSString *property, NSString *attribute, NSString *value) {
  __SArchiveXarAttributeSet((void *)file, property, attribute, value, (SArchiveXarAttributeSetter)xar_attr_set);
}

#pragma mark Xar Sub Document
NSString *SArchiveXarSubDocGetProperty(xar_subdoc_t doc, NSString *property) {
  return __SArchiveXarPropertyGet((void *)doc, property, (SArchiveXarPropertyGetter)xar_subdoc_prop_get);
}
void SArchiveXarSubDocSetProperty(xar_subdoc_t doc, NSString *property, NSString *value) {
  __SArchiveXarPropertySet((void *)doc, property, value, (SArchiveXarPropertySetter)xar_subdoc_prop_set);
}

NSString *SArchiveXarSubDocGetAttribute(xar_subdoc_t doc, NSString *property, NSString *attribute) {
  return __SArchiveXarAttributeGet((void *)doc, property, attribute, (SArchiveXarAttributeGetter)xar_subdoc_attr_get);
}
void SArchiveXarSubDocSetAttribute(xar_subdoc_t doc, NSString *property, NSString *attribute, NSString *value) {
  __SArchiveXarAttributeSet((void *)doc, property, attribute, value, (SArchiveXarAttributeSetter)xar_subdoc_attr_set);
}

