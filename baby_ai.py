import numpy as np
import tensorflow as tf
import cv2
import time
import requests


# =========================
# ESP8266 SETTINGS
# =========================

ESP_IP = "10.36.122.215"

# =========================
# LOAD AI MODEL
# =========================

print("Loading AI model...")

model = tf.keras.models.load_model("keras_model.h5")

with open("labels.txt", "r") as f:
    labels = f.read().splitlines()

print("AI model loaded!")
print("Labels:", labels)


# =========================
# FIND CAMERA
# =========================

print("Searching camera...")

camera = None

for i in range(3):

    cap = cv2.VideoCapture(i, cv2.CAP_DSHOW)

    time.sleep(1)

    if cap.isOpened():

        ret, frame = cap.read()

        if ret:
            print("Using camera:", i)
            camera = cap
            break

        cap.release()


if camera is None:
    print("No working camera found")
    exit()


cap = camera


# =========================
# CAMERA SETTINGS
# =========================

cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)


print("AI detection started!")


# =========================
# VARIABLES
# =========================

stable_result = ""
stable_start = time.time()
last_sent = ""


# =========================
# AI LOOP
# =========================

while True:


    ret, frame = cap.read()


    if not ret:
        print("Cannot get camera frame")
        continue



    # =========================
    # PREPARE IMAGE
    # =========================

    img = cv2.resize(frame, (224, 224))

    img = np.asarray(img)

    img = img.astype(np.float32)

    img = (img / 127.5) - 1

    img = np.expand_dims(img, axis=0)



    # =========================
    # AI PREDICTION
    # =========================

    prediction = model.predict(img, verbose=0)


    index = np.argmax(prediction)

    confidence = prediction[0][index]


    result = labels[index].strip().lower()


    print(result, round(float(confidence), 2))



    # =========================
    # STABLE DETECTION
    # =========================

    if result != stable_result:

        stable_result = result

        stable_start = time.time()



    # Wait 1 second before sending

    if time.time() - stable_start >= 1.0:


        if stable_result != last_sent:


            try:


                if "unsafe" in stable_result:


                    print("Sending DANGER...")


                    response = requests.get(
                        f"http://{ESP_IP}/danger",
                        timeout=2
                    )


                    if response.status_code == 200:

                        print("DANGER sent successfully")



                elif "safe" in stable_result:


                    print("Sending SAFE...")


                    response = requests.get(
                        f"http://{ESP_IP}/safe",
                        timeout=2
                    )


                    if response.status_code == 200:

                        print("SAFE sent successfully")



                last_sent = stable_result


                # small delay after sending

                time.sleep(0.5)



            except Exception as e:

                print("ESP8266 connection failed:", e)




    # =========================
    # DISPLAY RESULT
    # =========================

    if "unsafe" in result:

        text = "UNSAFE"

        color = (0, 0, 255)


    else:

        text = "SAFE"

        color = (0, 255, 0)



    cv2.putText(
        frame,
        text,
        (20, 40),
        cv2.FONT_HERSHEY_SIMPLEX,
        1,
        color,
        2
    )



    cv2.imshow(
        "Baby AI Camera",
        frame
    )



    # Press Q to quit

    if cv2.waitKey(1) & 0xFF == ord('q'):

        break



# =========================
# CLOSE
# =========================

cap.release()

cv2.destroyAllWindows()

print("Baby AI stopped")