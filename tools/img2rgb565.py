import cv2

WIDTH = 320
HEIGHT = 240

def img_to_raw(input_bmp, output_raw):
    img = cv2.imread(input_bmp)
    # img = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)    # 對應 Arduino rotation=0
    if img is None:
        print("Cannot open image")
        return

    # 縮放到 TFT 尺寸
    img = cv2.resize(img, (WIDTH, HEIGHT), interpolation=cv2.INTER_AREA)

    # BGR → RGB565
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    with open(output_raw, 'wb') as f:
        for y in range(HEIGHT):
            for x in range(WIDTH):
                r,g,b = img_rgb[y,x]
                rgb565 = int(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))
                f.write(rgb565.to_bytes(2,'big'))  # Arduino UNO + drawPixel 需 big endian

    print("RAW conversion done")

if __name__ == "__main__":
    import sys

    if len(sys.argv) < 3:
        print("Usage: python img2rgb565.py input.png output.raw")
        exit(1)

    img_to_raw(sys.argv[1], sys.argv[2])