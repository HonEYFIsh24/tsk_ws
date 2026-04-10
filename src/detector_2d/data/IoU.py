import cv2

def computeIoU(bbox1,bbox2):
    x_overlap = max(0,min(bbox1[2],bbox2[2])-max(bbox1[0],bbox2[0]))
    y_overlap = max(0,min(bbox1[3],bbox2[3])-max(bbox1[1],bbox2[1]))
    intersection = x_overlap*y_overlap
    # print(intersection)
    union = (bbox2[2]-bbox2[0])*(bbox2[3]-bbox2[1])+(bbox1[2]-bbox1[0])*(bbox1[3]-bbox1[1])-intersection
    # print(union)
    IoU = intersection/union
    return IoU
    
def main():
    timestamp = 9204
    image_raw = cv2.imread('/home/bowen/PythonProject/nanodet/catkin_ws/src/detector_2d/data/'+str(timestamp)+'_raw.jpg')
    imgWidth = image_raw.cols
    imgHeight = image_raw.rows
    with open('/home/bowen/PythonProject/nanodet/catkin_ws/src/detector_2d/data/'+str(timestamp)+'_det.txt', 'r') as file:
        lines_det = file.readlines()
    with open('/home/bowen/PythonProject/nanodet/catkin_ws/src/detector_2d/data/'+str(timestamp)+'_gt.txt', 'r') as file:
        lines_gt = file.readlines()
    for line_gt in lines_gt:
        _,gtCenterXRatio,gtCenterYRatio,gtCenterWidthRatio,gtCenterHeightRatio = line_gt.split()
        gtCenterX = imgWidth*float(gtCenterXRatio)
        gtCenterY = imgHeight*float(gtCenterYRatio)
        gtWidth = imgWidth*float(gtCenterWidthRatio)
        gtHeight = imgHeight*float(gtCenterHeightRatio)
        gtMinX = int(gtCenterX-gtWidth/2)
        gtMinY = int(gtCenterY-gtHeight/2)
        gtMaxX = int(gtCenterX+gtWidth/2)
        gtMaxY = int(gtCenterY+gtHeight/2)
        bbox1 = [gtMinX,gtMinY,gtMaxX,gtMaxY]
        for line_det in lines_det:
            x0,y0,x1,y1 = line_det.split()
            bbox2 = [x0,y0,x1,y1]
            IoU = computeIoU(bbox1, bbox2)
            if IoU > 0.5:
                print("Matched!")
                print('Ground Truth: ',bbox1)
                print('Detect: ',bbox2)
                print('IoU: ',IoU)

if __name__ == '__main__':
    main()
