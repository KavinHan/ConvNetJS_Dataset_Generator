

## addons
- ofxXmlSettings

## How to use?
1. make ```your_own_image_folder_name``` folder to ```bin/data/```
2. your_own_image_folder_name folder structure
```
bin/data/your_own_image_folder_name/
/label image folder/all_same_label_images.jpg||.png
/label image folder/all_same_label_images.jpg||.png
/...
```
3. change bin/data/setting.xml ``` <INPUT>your_own_image_folder_name</INPUT> ```
4. change bin/data/setting.xml ``` <OUTPUT>dataset_output_folder_name</OUTPUT> ```
5. change bin/data/setting.xml ``` <PREFIX>dataset_prefix_name</PREFIX> ```
6. run project, you will get dataset_batch_N.png and dataset_labels.js at ```bin/data/output/``` folder

## Support
- Just support grayscale dataset output now

## Todo
- support color dataset
