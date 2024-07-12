﻿using System;
using System.IO;
using System.Threading;
using Microsoft.AspNetCore.Http;
using Newtonsoft.Json;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.Formats.Gif;
using SixLabors.ImageSharp.Formats.Png;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using SixLabors.ImageSharp.Processing.Processors.Transforms;

namespace Utilities;

public static class Extensions
{
    public static string _sha1;
    public static string _hostname;

    public static byte[] ImageToByte(this Image<Rgba32> img, HttpRequest context, bool autoResize = true, IImageFormat format = null, bool autoDispose = false, object embedMeta = null)
    {
        if (format == null) format = PngFormat.Instance;
        var shouldResize = (context?.Query.ContainsKey("resize") ?? false) && autoResize;
        var didResize = false;
        if (shouldResize)
        {
            string userResizeAmount = context.Query["resize"];
            var resizeAmount = decimal.Parse(userResizeAmount);
            if (resizeAmount != 1 && img.Height * resizeAmount < 50000 && img.Width * resizeAmount < 50000)
            {
                img = img.Clone(c => c.Resize(new ResizeOptions
                {
                    Mode = ResizeMode.Stretch,
                    Sampler = new NearestNeighborResampler(),
                    Size = new Size((int) (img.Width * resizeAmount), (int) (img.Height * resizeAmount))
                }));
                didResize = true;
            }
        }

        byte[] imgData = null;
        using (var mem = new MemoryStream())
        {
            lock (img)
            {
                if (format == PngFormat.Instance)
                    new PngEncoder().Encode(img, mem);
                else if (format == GifFormat.Instance)
                    new GifEncoder().Encode(img, mem);
                else
                    img.Save(mem, format);
            }

            imgData = mem.ToArray();
        }

        byte[] jsonPayload = null;
        using (var mem = new MemoryStream())
        {
            using (var writer = new BinaryWriter(mem))
                writer.Write(JsonConvert.SerializeObject(new
                {
                    notice = "Generated by MapleStory.IO",
                    container = _hostname ?? "Debug",
                    gitHash = _sha1,
                    url = context == null ? "Debug" : context.Path.ToString(),
                    createdAt = DateTime.UtcNow,
                    meta = embedMeta
                }));

            jsonPayload = mem.ToArray();
        }

        if (autoDispose || didResize)
            ThreadPool.QueueUserWorkItem(s =>
            {
                try
                {
                    img.Dispose();
                }
                catch (Exception) { }
            });

        var totalPayload = new byte[jsonPayload.Length + imgData.Length + 1];
        Buffer.BlockCopy(imgData, 0, totalPayload, 0, imgData.Length);
        Buffer.BlockCopy(jsonPayload, 0, totalPayload, imgData.Length + 1, jsonPayload.Length);

        return totalPayload;
    }
    
}