/*
 * This file is part of the espx-dancing-led distribution (https://github.com/kluevandrew/espx-dancing-led).
 * Copyright (c) 2019, Kluev Andrew <kluev.andrew@gmail.com>.
 *
 * espx-dancing-led is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You are free to:
 *   Share — copy and redistribute the material in any medium or format
 *   Adapt — remix, transform, and build upon the material
 *
 * Under the following terms:
 *   Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
 *   NonCommercial — You may not use the material for commercial purposes.
 *   ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
 *   No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
 *
 * For additional information, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 */
const path = require('path');
const webpack = require('webpack');
const ExtractTextPlugin = require('extract-text-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CleanWebpackPlugin = require('clean-webpack-plugin');
const CopyPlugin = require('copy-webpack-plugin');
const OptimizeCssAssetsPlugin = require('optimize-css-assets-webpack-plugin');
const Autoprefixer = require('autoprefixer');

const PATHS = {
    root: path.resolve(__dirname, './'),
    source: path.join(__dirname, './ui'),
    build: path.join(__dirname, './data')
};

module.exports = {
    mode: 'production',
    context: PATHS.root + '/ui',
    entry: {
        'esp8266-web-fastled-music': [PATHS.root + '/ui/app.ts', PATHS.root + '/ui/sass/app.scss'],
    },
    output: {
        path: PATHS.build,
        filename: 'app.js',
        publicPath: '/'
    },
    resolve: {
        extensions: ['.ts', '.js', '.css', '.scss', '.html', '.svg', '.jpg', '.jpeg', '.png', '.gif'],
    },
    module: {
        rules: [
            {
                test: /\.ts$/,
                loader: 'ts-loader',
                exclude: /node_modules/,
            },
            {
                test: /\.(css|scss)$/,
                loader: ExtractTextPlugin.extract({
                    use: 'css-loader!sass-loader',
                    fallback: 'style-loader',
                }),
            },
            {
                test: /\.(html)$/,
                use: {
                    loader: 'html-loader',
                    options: {
                        attrs: [':data-src']
                    }
                }
            }
        ],
    },
    plugins: [
        new webpack.ProgressPlugin(),
        new CleanWebpackPlugin(),
        new HtmlWebpackPlugin({
            template: PATHS.root + '/ui/index.html'
        }),
        Autoprefixer,
        new ExtractTextPlugin({
            filename: 'app.css',
            allChunks: true,
        }),
        new OptimizeCssAssetsPlugin({
            assetNameRegExp: /\.css$/g,
            cssProcessor: require('cssnano'),
            cssProcessorPluginOptions: {
                preset: ['default', {discardComments: {removeAll: true}}],
            },
            canPrint: true
        }),
        new CopyPlugin([
            {from: 'favicon.ico', to: 'favicon.ico', force: true},
        ])
    ],
};
