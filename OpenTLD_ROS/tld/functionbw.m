        function functionbw(message)
            % BWIMGCALLBACK - Execute tasks when a new image
            % (sensor_msgs/Image) is received.
            global q;     
            width = message.getWidth();
            height = message.getHeight();
            imgCol = typecast(message.getData().array(), 'uint8');
            img = reshape(imgCol(69:end), width, height)';
            imgRgb = cat(3, img, img, img);
            q=imgRgb;
            end
        
