import os
import sys
import argparse
import cv2
import numpy as np
import time
import math


#
# TODO:
#
# For each image, record some stats:
#   sum of pixel values
#   centroid == sum_i,j(img[i,j]*(i,j)) / sum_ij(img[i,j])
#   variance equivalent
#
# To find thresholds, maybe look for peaks in histogram of total sum?
# Also, "all-on" and "all-off" will have large spatial variance
#
# Maybe simple 1-D k-means on the sum of pixel values to split? 
# Use k=3 if we need to detect "all-on", and 2 otherwise.
#

# Functions
def read_image(filename):
    img = cv2.imread(filename)
    return img

# Get L2 difference between two images
def img_dist(img1, img2):
    return np.linalg.norm(np.array(img1)-np.array(img2)) / img1.size

def img_norm(img):
    return np.linalg.norm(np.array(img)) / img.size

def img_avg(img):
    return img.sum() / img.size

def centroid(img):
    row_sum = 0
    col_sum = 0
    sum = img.sum()

    for row in range(img.shape[0]):
        for col in range(img.shape[1]):
            for chan in range(img.shape[2]):
                row_sum += img[row, col, chan] * row
                col_sum += img[row, col, chan] * col

    return (row_sum / sum, col_sum / sum)

# Make a class representing a directory of images to use as a dataset

# Operations:
#
# Read dataset from directory -- assumes "dark" images between real ones
# Get number of entries
# Get filename for representative image for each entry
# Get average image for each entry span 

class ImageDataset(object):
    def __init__(self, directory):
        self.filenames = [os.path.join(directory, f) for f in self.get_directory_entries(directory, '.png')]
        self.avg_img_levels = np.array([])
        self.spans = []

    # Raw entry methods        
    def num_entries(self):
        return len(self.filenames)

    def num_spans(self):
        return len(self.spans)

    def entry_filename(self, index):
        return self.filenames[index]

    # Span methods
    def span_image(self, index):
        span = self.spans[index]
        img_index = (span[0] + span[1]) // 2
        return self.filenames[img_index]

    def avg_span_image(self, index):
        return None

    def find_image_spans(self):
        # a span is the [begin, end) indices for a span of images belonging to the same "thing"
        # doesn't include background images
        mean_avg = self.avg_img_levels.mean()

        # now find all contiguous spans of images with avg > mean_avg
        spans = []
        span_start = None  # None means not currently in a span
        for index in range(self.avg_img_levels.size):
            is_good = self.avg_img_levels[index] > mean_avg
            if is_good and span_start is None:
                span_start = index  # Start a new span
            elif not is_good and span_start is not None:
                spans.append((span_start, index))
                span_start = None  # End a span

        # Add final span if there is one:
        if span_start is not None:
            spans.append((span_start, self.avg_img_levels.size))
        self.spans = spans

    def span_centroid(self, index):
        filename = self.span_image(index)
        img = read_image(filename)
        return centroid(img)
    
    def scan_files(self):
        avgs = []
        for filename in self.filenames:
            img = read_image(filename)
            avg = img_avg(img)
            avgs.append(avg)
        self.avg_img_levels = np.array(avgs)

    def get_directory_entries(self, directory, extension=None):
        files = os.listdir(directory)
        if extension:
            files = [f for f in files if f.endswith(extension)]
        return sorted(files)
