declare interface ImageData {
    height: number
    width: number
    mime: string
    base64: string
}

declare type Metadata = { [key: string]: string | Metadata }

declare interface PreviewAndMetadata {
    exifData: Metadata
    image: ImageData
}

declare interface Options {
    ignoreMetadata?: Array<string>,
    ignoreMetadataWithoutName?: Boolean,
    includeBase64?: Boolean
}

declare function getPreviewAndMetadata(path: string, options?: Options): Promise<PreviewAndMetadata>

export {
    getPreviewAndMetadata
}