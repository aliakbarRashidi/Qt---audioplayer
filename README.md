# Mini sound editor in Qt

The goal of this lab was to provide a simplified sound editor in order to be familiar with some high-level event handling mechanisms (signal / slot) and low level (Event).

The difficulty of this work was the sound card management and constraints related to a real-time app. One must be able to "play" a song while ensuring the responsiveness of the GUI and its modification according to the progress of the song. To facilitate development, I used the module Phonon Qt4 which  contains namespaces and classes for multimedia functionalities.

Here you have a screenshot of the result:

![img](https://s19.postimg.org/68cs1s4fn/audioplayer.png "Mini sound editor in Qt")