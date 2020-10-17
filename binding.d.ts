declare interface ImageData {
    height: number,
    width: number,
    mime: string,
    base64: string
}

declare interface PreviewAndMetadata {
    exifData: { [key: string]: string; },
    image: ImageData
}

declare function getPreviewAndMetadata(path: string): Promise<PreviewAndMetadata>

export {
    getPreviewAndMetadata
}