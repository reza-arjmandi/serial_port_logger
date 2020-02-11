# Unit testing async_io library

## Test drive `Session_controller`
The `Session_controller` used by `TCP_server` and `TCP_client` and it must maintain all sessions, start and stop operations of sessions must be done though `Session_controller`
I've added 8 test cases in [`Session_controller_tests.cpp`](Session_controller_tests.cpp).

## Test drive `Session`
The `Session` contains the policy of conversation between `TCP_server` and `TCP_client`. It determines when chain of send or receive must be stopped.\
I use short read/write technique and in every send/receive operation we transfer a chunk of data called packet. The chain of async send or receive breaks when packet is equal to a specified packet called `end_packet`.\
I've implement 14 test cases in [`Session_tests.cpp`](Session_tests).