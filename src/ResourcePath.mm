#include "FileManager.h"
#import <Foundation/Foundation.h>

namespace FileManager
{
    std::string ResourcePath(void)
    {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

        std::string rpath;
        NSBundle* bundle = [NSBundle mainBundle];

        if (bundle == nil)
        {
            #if defined(DEBUG)
                NSLog(@"bundle is nil... thus no resources path can be found.");
            #endif
        }
        else
        {
            NSString* path = [bundle resourcePath];
            rpath = [path UTF8String] + std::string("/");
        }

        [pool drain];

        return rpath;
    }
}
