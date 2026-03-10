#
//  Arduino_Trigger_Python.py
//
//
//  Created by Terry Jachimiak on 3/10/26.
//

# me - this DAT.
#
# dat - the changed DAT
# rows - a list of row indices
# cols - a list of column indices
# cells - the list of cells that have changed content
# prev - the list of previous string contents of the changed cells
#
# Make sure the corresponding toggle is enabled in the DAT Execute DAT.
#
# If rows or columns are deleted, sizeChange will be called instead of row/col/cellChange.

# me - this DAT
# dat - the changed DAT
import time

def ignoreForSeconds(seconds):
    """
    Call this function to ignore messages for a specified duration
    """
    me.store('ignoreUntil', time.time() + seconds)
    print(f"Ignoring serial messages for {seconds} seconds")
    
def stopIgnoring():
    """
    Call this to immediately stop ignoring messages
    """
    me.store('ignoreUntil', 0)
    print("No longer ignoring serial messages")
    
def onTableChange(dat):
    # Fetch ignoreUntil time (default 0)
    ignoreUntil = me.fetch('ignoreUntil', 0)
    print(ignoreUntil)
    currentTime = time.time()
    print(currentTime)
    
    # If still in ignore period, exit early
    if currentTime < ignoreUntil:
        return
        
    # Process only if there’s at least one data row
    if dat.numRows > 1:
        message = dat[1, 0].val.strip()
        
        print(f"Received: {message}")  # Debug output
        
        if message == "On1":
            print("Triggering Pattern CHOP!")
            op('electric').par.cue.pulse()
            op('oscout1').send('/eos/cue/20/fire')
            op('oscout2').send('/cue/4.12/start')
           
            # Ignore further messages for 40 seconds
            # This counts for 20 seconds of the clip and 20 more seconds as people walk through
            #ignoreForSeconds(40)
        
def onRowChange(dat, rows):
    return

def onColChange(dat, cols):
    return

def onCellChange(dat, cells, prev):
    return

def onSizeChange(dat):
    return

