import json
import time
import sys
import serial

def initPorts():
    controlOutputPort = serial.Serial(port = "COM6", baudrate=57600, timeout=1)
    return controlOutputPort

def decodeLoadCellData(serialData):
    raw = serialData.decode().splitlines()
    if (len(raw)>0):
        rawSplit = raw[0].split(':')
        if (len(rawSplit)==2):
            return(rawSplit[1])
        elif (len(rawSplit)==1):
            return(rawSplit[0])

    return('')

def dumpSerialData(controlOutputPort):
    try: 
        outputBuffer = []
        while 1:
            controlOutputPort.reset_input_buffer()
            loadCellValue = decodeLoadCellData(controlOutputPort.readline())
            if len(loadCellValue)>0:
                print(loadCellValue)
                outputBuffer.append(float(loadCellValue))
                with open('../user-interface/src/tracking_output.json', 'w', encoding='utf-8') as f:
                    json.dump((outputBuffer), f, ensure_ascii=False, indent=4)
    except (KeyboardInterrupt, SystemExit) as exErr:
        print("Inturrupt: ", exErr)
        return


if __name__ == '__main__':
    controlOutputPort = initPorts()

    dumpSerialData(controlOutputPort)