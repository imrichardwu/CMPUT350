// Code Review
// Anything wrong with this code?

// return 0    => everything OK (e.g., allow ssh connection ...)
// return != 0 => error
static OSStatus
SSLVerifySignedServerKeyExchange(SSLContext *ctx, bool isRsa,
                                 SSLBuffer signedParams,
                                 uint8_t *signature, UInt16 signatureLen)
{
  OSStatus err;
  ...
 
    if ((err = SSLHashSHA1.update(&hashCtx, &serverRandom)) != 0) 
      goto fail;

    if ((err = SSLHashSHA1.update(&hashCtx, &signedParams)) != 0) 
      goto fail;
      goto fail;

    if ((err = SSLHashSHA1.final(&hashCtx, &hashOut)) != 0) 
      goto fail;

    ...
 
fail:
    SSLFreeBuffer(&signedHashes);
    SSLFreeBuffer(&hashCtx);
    return err;
}







/*

This is Apple's IOS SSL library bug ...

Problem: two gotos in a row ...

=> Networking connections get accepted even if they lack credentials!
   (err = 0 upon return from fail:)
   
OUCH OUCH OUCH ...



Unreachable code like this will be detected by

clang++ -Wunreachable-code ...

g++'s -Wunreachable-code option doesn't work :-(


Lessons learned:

- Implement unit-tests to test your code at the function level.
  After each code change, run unit-tests to ensure you didn't break any
  previous functionality

- The more important the code is, the more it must be scrutinized.  (E.g.,
  critical modules such as networking and encryption require extensive
  testing)
  
- Add assert statements which at runtime check crucially important, such as
  error != 0 when reaching fail: in this example. Don't switch assertions off
  in non-time-critical code

- Always include if/else/while/for code in { } - even if you only execute one
  line. Later additions (perhaps by someone else) may turn one-liners into
  more-liners, which now require { }

- Regular code-reviews can catch errors such as this one and more subtle bugs

*/
