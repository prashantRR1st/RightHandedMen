import json
import time
import sys
import serial

def initPorts():
    imuPort = serial.Serial(port = "COM5", baudrate=57600, timeout=1)
    loadCellPort = serial.Serial(port = "COM4", baudrate=57600, timeout=1)
    return loadCellPort, imuPort

def decodeLoadCellData(serialData):
    raw = serialData.decode().splitlines()
    if (len(raw)>0):
        rawSplit = raw[0].split(',')
        if (len(rawSplit)==2):
            return(rawSplit[1]) 
    return('')


def readSerial(loadCellPort, imuPort):
    jsonData = {
        "data": []
    }
    try:
        loopDelta = float(0)
        while 1:
            loopStartTime = time.time()
            loadCellValue = decodeLoadCellData(loadCellPort.readline())
            imuLine = imuPort.readline()
            imuExtract = imuLine.decode().split(',')
            
            if len(loadCellValue) > 0 and len(imuExtract) == 3:
                dataObj = {
                    "loadCell": float(loadCellValue),
                    "imu": {
                        "ax": float(imuExtract[0].split('=')[1]),
                        "ay": float(imuExtract[1]),
                        "az": float(imuExtract[2])
                    },
                    "millis": time.time()*1000
                }
                print(dataObj)
                jsonData["data"].append(dataObj)
                if loopDelta > 0.5:
                    loopDelta = 0 
                time.sleep(0.5-loopDelta)
            else:
                print("Waiting for data \n")
            
            loopDelta = time.time() - loopStartTime

    except (KeyboardInterrupt, SystemExit) as exErr:
        print("Inturrupt: ", exErr)
        return jsonData


def displayData(rawData):
    for idx, dataObj in enumerate(rawData["data"]):
        print(idx, " loadCell: ", dataObj["loadCell"] ," imu_ax: ", dataObj["imu"]["ax"], " imu_ay: ", dataObj["imu"]["ay"], " imu_az: ", dataObj["imu"]["az"], " millis: ", dataObj["millis"] )

def processAndStoreRawData(rawData, startIndex, contactIndex, endIndex):
    result = []
    for i in range(contactIndex-1, startIndex-1, -1):
        # rawData[i]["loadCell"] = (2*rawData[i+1]["loadCell"]*rawData[i+1]["imu"]["ax"]-rawData[i+2]["loadCell"]*rawData[i+2]["imu"]["ax"])/(rawData[i]["imu"]["ax"])
        rawData[i]["loadCell"] = (1/rawData[i]["imu"]["ax"])*(rawData[i+1]["loadCell"]*rawData[i+1]["imu"]["ax"]-(((rawData[i+2]["millis"]-rawData[i+1]["millis"])*(rawData[i+2]["loadCell"]*rawData[i+2]["imu"]["ax"]-rawData[i+1]["loadCell"]*rawData[i+1]["imu"]["ax"]))/(rawData[i+1]["millis"]-rawData[i]["millis"])))
    
    for idx in range(startIndex, endIndex+1):
        result.append(rawData[idx]["loadCell"])        

    with open('result.json', 'w', encoding='utf-8') as f:
        json.dump(result, f, ensure_ascii=False, indent=4)
    

if __name__ == '__main__':

    loadCellPort, imuPort = initPorts()

    rawData = readSerial(loadCellPort, imuPort)

    displayData(rawData)

    startIndex = input("enter the START index")
    contactIndex = input("enter the CONTACT index")
    endIndex = input("enter the END index")

    processAndStoreRawData(rawData["data"], int(startIndex), int(contactIndex), int(endIndex))