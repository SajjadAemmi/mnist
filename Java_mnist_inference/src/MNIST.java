import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.Raster;
import java.io.File;
import java.io.IOException;
import java.util.Map;
import javax.imageio.ImageIO;
//import org.opencv.core.*;
//import org.opencv.imgcodecs.Imgcodecs;
//import org.opencv.imgproc.Imgproc;
import ai.onnxruntime.OnnxTensor;
import ai.onnxruntime.OrtEnvironment;
import ai.onnxruntime.OrtException;
import ai.onnxruntime.OrtSession;
import ai.onnxruntime.OrtSession.SessionOptions;
import ai.onnxruntime.OrtSession.Result;

public class MNIST {
    private JFrame frame;
    private JLabel predictionLabel;
    private JButton loadImageButton;
    private BufferedImage inputImage;

    private OrtEnvironment env;
    private OrtSession session;

    public MNIST() {
        initializeUI();
        initializeONNXModel();
    }

    private void initializeUI() {
        frame = new JFrame("MNIST Prediction");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 200);
        frame.setLayout(new BorderLayout());

        predictionLabel = new JLabel("Prediction: ");
        loadImageButton = new JButton("Load Image");

        loadImageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                if (fileChooser.showOpenDialog(frame) == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile();
                    try {
                        inputImage = ImageIO.read(selectedFile);
                        predict();
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                }
            }
        });

        frame.add(loadImageButton, BorderLayout.NORTH);
        frame.add(predictionLabel, BorderLayout.CENTER);

        frame.setVisible(true);
    }

    private void initializeONNXModel() {
        try {
            env = OrtEnvironment.getEnvironment();
            session = env.createSession("weights/mnist.onnx", new SessionOptions());
        } catch (OrtException e) {
            e.printStackTrace();
        }
    }

    private void predict() {
        try {
            float[][][][] inputFloats = preprocessInput(inputImage);
            OnnxTensor inputTensor = OnnxTensor.createTensor(env, inputFloats);
            Map<String, OnnxTensor> inputs = Map.of("0", inputTensor);
            Result result = session.run(inputs);
            float[][] output = (float[][]) result.get(0).getValue();

            int predictedLabel = argmax(output);
            predictionLabel.setText("Prediction: " + predictedLabel);

            inputTensor.close();
            result.close();
        } catch (OrtException e) {
            e.printStackTrace();
        }
    }

    private float[][][][] preprocessInput(BufferedImage image) {
        int width = 28;
        int height = 28;
        // 1. Resize the input image to the desired width and height
        BufferedImage resizedImage = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_GRAY);
        Graphics2D g = resizedImage.createGraphics();
        g.drawImage(image, 0, 0, width, height, null);
        g.dispose();

        // 3. Normalize the pixel values to a range suitable for the model
        float[] inputFloats = new float[width * height];
        Raster raster = resizedImage.getRaster();
        DataBufferByte dataBuffer = (DataBufferByte) raster.getDataBuffer();
        byte[] pixels = dataBuffer.getData();

        for (int i = 0; i < pixels.length; i++) {
            if((pixels[i] & 0xFF) >= 127)
                inputFloats[i] = 1.0f;
            else
                inputFloats[i] = 0.0f;
        }

        // 4. Create a 4D float array representing the image
        float[][][][] inputTensorData = new float[1][1][width][height];

        // Copy the preprocessed image data into the 4D array
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                inputTensorData[0][0][x][y] = inputFloats[x * width + y];
            }
        }

        return inputTensorData;
    }

//    private Mat preprocessInput(BufferedImage image) {
//        int width = 28;
//        int height = 28;
//
//        // 1. Resize the input image to the desired width and height
//        Mat resizedImage = new Mat();
//        Mat srcImage = new Mat();
//
//        // Convert BufferedImage to Mat
//        BufferedImageToMat(image, srcImage);
//
//        // Resize the image
//        Imgproc.resize(srcImage, resizedImage, new Size(width, height));
//
//        // 2. Convert the image to grayscale (if it's not already)
//        if (resizedImage.channels() > 1) {
//            Imgproc.cvtColor(resizedImage, resizedImage, Imgproc.COLOR_BGR2GRAY);
//        }
//
//        // 3. Normalize the pixel values to a range suitable for the model
//        Mat normalizedImage = new Mat();
//        resizedImage.convertTo(normalizedImage, CvType.CV_32F, 1.0 / 255.0);
//
//        // 4. Expand dimensions to match the required shape [1, 1, width, height]
//        Mat finalImage = new Mat(1, 1, CvType.CV_32FC1);
//        normalizedImage.reshape(1, 1).copyTo(finalImage);
//
//        return finalImage;
//    }

//    private void BufferedImageToMat(BufferedImage bufferedImage, Mat mat) {
//        byte[] pixels = ((DataBufferByte) bufferedImage.getRaster().getDataBuffer()).getData();
//        mat.put(0, 0, pixels);
//    }

    private int argmax(float[][] array) {
        if (array == null || array.length == 0) {
            throw new IllegalArgumentException("Input array is empty or null.");
        }

        int maxIndex = 0;
        float maxValue = array[0][0];

        for (int i = 1; i < array[0].length; i++) {
            if (array[0][i] > maxValue) {
                maxValue = array[0][i];
                maxIndex = i;
            }
        }

        return maxIndex;
    }


    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new MNIST();
            }
        });
    }
}
