import cv2

print("Checking cameras...")

for i in range(5):
    cap = cv2.VideoCapture(i)

    if cap.isOpened():
        ret, frame = cap.read()

        if ret:
            print("Camera", i, "WORKING")
        else:
            print("Camera", i, "opened but no image")

        cap.release()

    else:
        print("Camera", i, "not found")