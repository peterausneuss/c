# Inhaltsverzeichnis der Beispiele im Verzeichnis #

## c_basics_01.c ##

        asctime(const struct tm *timeptr): No formatting options.
        Always outputs in "Www Mmm dd hh:mm:ss yyyy\n" format.

        strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr):
        Highly configurable formatting via the format string. 

        	        printf	                                        fprintf
        Output to	Standard output (stdout) by default.	        A specified FILE stream (e.g., stdout,                                                         stderr, or a file).
        