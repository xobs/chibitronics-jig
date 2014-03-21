Chibitronics Test Jig
=====================

This software runs in the factory that produces Chibitronics Circuit Stickers.

It talks to the stickers via a custom jig.  Basically, it bit-bangs TWI over a
kind of SPI system, with an extra pin that's responsible for controling TWI direction.

See also https://github.com/xobs/avrdude-chibitronics
