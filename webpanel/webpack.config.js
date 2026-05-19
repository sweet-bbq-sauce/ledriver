const path = require('path');

const HtmlWebpackPlugin = require('html-webpack-plugin');
const CompressionPlugin = require('compression-webpack-plugin');

module.exports = {
    entry: './src/index.ts',

    output: {
        path: path.resolve(__dirname, 'dist/public'),
        filename: 'app.js',
        clean: true,
    },

    resolve: {
        extensions: ['.ts', '.js'],
    },

    module: {
        rules: [
            {
                test: /\.ts$/,
                use: 'ts-loader',
                exclude: /node_modules/,
            },
            {
                test: /\.css$/,
                type: 'asset/source',
            },
        ],
    },

    plugins: [
        new HtmlWebpackPlugin({
            template: './src/index.html',
            filename: 'index.html',
            inject: 'body',
            minify: {
                collapseWhitespace: true,
                removeComments: true,
                removeRedundantAttributes: true,
            },
        }),

        new CompressionPlugin({
            algorithm: 'gzip',
            filename: '[path][base].gz',
            test: /\.(html|js|css|svg|json|ico)$/,
            threshold: 0,
            minRatio: 1,
            deleteOriginalAssets: true,
            compressionOptions: {
                level: 9,
            }
        }),
    ],

    optimization: {
        minimize: true,
    },

    devtool: false,

    performance: {
        hints: false,
    },
};
