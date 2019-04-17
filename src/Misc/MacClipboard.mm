#include "Misc/MacClipboard.h"
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

void SetMacClipboardText(const sf::String& text)
{
    NSPasteboard* pb = [NSPasteboard generalPasteboard];
    [pb clearContents];

    std::basic_string<sf::Uint8> utf8 = text.toUtf8();
    NSString* pbData = [[NSString alloc] initWithBytes:utf8.data()
                                                length:utf8.length()
                                              encoding:NSUTF8StringEncoding];
    [pb setString:pbData forType:NSPasteboardTypeString];
    [pbData release];
}

sf::String GetMacClipboardText()
{
    NSPasteboard* pb = [NSPasteboard generalPasteboard];
    NSString* pbData = [pb stringForType:NSPasteboardTypeString];

    const char* utf8 = [pbData cStringUsingEncoding:NSUTF8StringEncoding];
    NSUInteger length = [pbData lengthOfBytesUsingEncoding:NSUTF8StringEncoding];

    return sf::String::fromUtf8(utf8, utf8 + length);
}
