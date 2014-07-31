import sys
import logging

# open file in unbuffered mode so it get written asap, in case of later crash
# due to underlying C code
stream = open('tympan.log', 'a', 0)
logging.basicConfig(stream=stream, level=logging.DEBUG,
                    format='%(levelname)s:%(asctime)s - %(name)s - %(message)s')

import tympan.altimetry.process_altimetry as tyalti

if __name__ == '__main__':
    if len(sys.argv) != 3:
        err = "process_site_altimetry.py called with bad arguments."
        logging.error("%s Couldn't process altimetry.", err)
        sys.exit(-1) # XXX to be improved
    # read command-line argument
    input_proj = sys.argv[1]
    # process altimetry
    try:
        tyalti.process_site_altimetry(input_project=input_proj)
    except:
        logging.exception("process_altimetry.py couldn't process the altimetry.")
        sys.exit(-1)
    sys.exit(0)
