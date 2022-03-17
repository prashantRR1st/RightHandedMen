import json
import time
import sys
import serial

def initPorts():
    imuPort = serial.Serial(port = "COM5", baudrate=57600, timeout=1)
    loadCellPort = serial.Serial(port = "COM6", baudrate=57600, timeout=1)
    return loadCellPort, imuPort

def decodeLoadCellData(serialData):
    raw = serialData.decode().splitlines()
    if (len(raw)>0):
        rawSplit = raw[0].split(':')
        if (len(rawSplit)==2):
            return(rawSplit[1])
        elif (len(rawSplit)==1):
            return(rawSplit[0])

    return('')

def decodeIMUData(imuExtract):
    ax = imuExtract[0].split('=')
    if len(imuExtract) == 3 and len(ax)==2:
        return imuExtract[0].split('=')[1], imuExtract[1], imuExtract[2]
    return '', '', ''


def readSerial(loadCellPort, imuPort):
    jsonData = {
        "data": []
    }
    try:
        loopDelta = float(0)
        while 1:
            loopStartTime = time.time()
            loadCellPort.reset_input_buffer()
            loadCellValue = decodeLoadCellData(loadCellPort.readline())
            imuPort.reset_input_buffer()
            imuLine = imuPort.readline()
            ax, ay, az = decodeIMUData(imuLine.decode().split(','))
            
            if len(loadCellValue) > 0 and len(ax) > 0 and len(ay) > 0 and len(az) > 0:
                dataObj = {
                    "loadCell": float(loadCellValue),
                    "imu": {
                        "ax": float(ax)*9.81/230,
                        "ay": float(ay), # no need to convert
                        "az": float(az)
                    },
                    "millis": time.time()*1000
                }
                print(dataObj)
                jsonData["data"].append(dataObj)
                print(loopDelta)
                # if loopDelta > 0.5:
                #     loopDelta = 0 
                #time.sleep(0.5-loopDelta)
            else:
                print("Waiting for data \n")
            
            loopDelta = time.time() - loopStartTime

    except (KeyboardInterrupt, SystemExit) as exErr:
        print("Inturrupt: ", exErr)
        return jsonData


def displayData(rawData):
    for idx, dataObj in enumerate(rawData["data"]):
        print(idx, " loadCell: ", dataObj["loadCell"] ," imu_ax: ", dataObj["imu"]["ax"], " imu_ay: ", dataObj["imu"]["ay"], " imu_az: ", dataObj["imu"]["az"], " millis: ", dataObj["millis"] )

def backInterpolateFromContact(rawData, startIndex, contactIndex):
    for i in range(contactIndex-1, startIndex-1, -1):
        accn_i = 0.0001
        t_1_0_delta = 0.0001
        if (rawData[i]["imu"]["ax"] != 0.0):
            accn_i = rawData[i]["imu"]["ax"]
        if (rawData[i+1]["millis"]-rawData[i]["millis"] != 0.0):
            t_1_0_delta = rawData[i+1]["millis"]-rawData[i]["millis"]
        backInterpolatedValue = (1/accn_i)*(rawData[i+1]["loadCell"]*rawData[i+1]["imu"]["ax"]-(((rawData[i+2]["millis"]-rawData[i+1]["millis"])*(rawData[i+2]["loadCell"]*rawData[i+2]["imu"]["ax"]-rawData[i+1]["loadCell"]*rawData[i+1]["imu"]["ax"]))/(t_1_0_delta)))
        rawData[i]["loadCell"] = backInterpolatedValue if backInterpolatedValue>=0 else 0
    return rawData

def eliminateOutliersIQR(rawData): # to handle observed (sudden) sensor reading drop-offs
    for i in range (1, len(rawData)-2):
        movingAvg = (rawData[i+1]["loadCell"]+rawData[i-1]["loadCell"])/2
        if (abs(rawData[i]["loadCell"]) < 0.25*abs(movingAvg) or abs(rawData[i]["loadCell"]) > 1.75*abs(movingAvg)):
            rawData[i]["loadCell"] = movingAvg
    return rawData
            
def sendToFile(result):
    with open('../user-interface/src/generated_input.json', 'w') as f:
        json.dump(result, f, ensure_ascii=False, indent=4)

    with open('../user-interface/src/generated_input', 'ab') as f:
        for item in result['buffer']:
            f.write(int(item).to_bytes(4, byteorder='big', signed=True))


def processAndStoreRawData(rawData, startIndex, contactIndex, endIndex):
    result = {
        'buffer': [],
        'totalTime': rawData[endIndex]["millis"]-rawData[startIndex]["millis"]
    }

    rawData = eliminateOutliersIQR(rawData)
    rawData = backInterpolateFromContact(rawData, startIndex, contactIndex)
    
    offset = 100 #grams
    for idx in range(startIndex, endIndex+1):
        result['buffer'].append(int(rawData[idx]["loadCell"]+offset))   

    result['buffer'].append(0)
    sendToFile(result)

if __name__ == '__main__':

    loadCellPort, imuPort = initPorts()

    rawData = readSerial(loadCellPort, imuPort)

    displayData(rawData)

    startIndex = input("enter the START index")
    contactIndex = input("enter the CONTACT index")
    endIndex = input("enter the END index")

    processAndStoreRawData(rawData["data"], int(startIndex), int(contactIndex), int(endIndex))