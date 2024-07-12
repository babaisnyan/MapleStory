using System;
using System.IO;

namespace Utilities;

public class SubStream : Stream
{
    private readonly long _length;
    private readonly Stream _baseStream;
    private long _position;

    public SubStream(Stream baseStream, long offset, long length)
    {
        if (baseStream == null) throw new ArgumentNullException("baseStream");
        if (!baseStream.CanRead) throw new ArgumentException("can't read base stream");
        if (offset < 0) throw new ArgumentOutOfRangeException("offset");

        _baseStream = baseStream;
        _length = length;

        if (baseStream.CanSeek)
        {
            baseStream.Seek(offset, SeekOrigin.Current);
        }
        else
        {
            // read it manually...
            const int bufferSize = 512;
            var buffer = new byte[bufferSize];
            while (offset > 0)
            {
                var read = baseStream.Read(buffer, 0, offset < bufferSize ? (int) offset : bufferSize);
                offset -= read;
            }
        }
    }

    public override long Length
    {
        get
        {
            CheckDisposed();
            return _length;
        }
    }

    public override bool CanRead
    {
        get
        {
            CheckDisposed();
            return true;
        }
    }

    public override bool CanWrite
    {
        get
        {
            CheckDisposed();
            return false;
        }
    }

    public override bool CanSeek
    {
        get
        {
            CheckDisposed();
            return false;
        }
    }

    public override long Position
    {
        get
        {
            CheckDisposed();
            return _position;
        }
        set => throw new NotSupportedException();
    }

    public override int Read(byte[] buffer, int offset, int count)
    {
        CheckDisposed();
        var remaining = _length - _position;
        if (remaining <= 0) return 0;
        if (remaining < count) count = (int) remaining;
        var read = _baseStream.Read(buffer, offset, count);
        _position += read;
        return read;
    }

    public override long Seek(long offset, SeekOrigin origin)
    {
        throw new NotSupportedException();
    }

    public override void SetLength(long value)
    {
        throw new NotSupportedException();
    }

    public override void Flush()
    {
        CheckDisposed();
        _baseStream.Flush();
    }

    public override void Write(byte[] buffer, int offset, int count)
    {
        throw new NotImplementedException();
    }

    protected override void Dispose(bool disposing)
    {
        base.Dispose(disposing);
    }

    private void CheckDisposed()
    {
        if (_baseStream == null) throw new ObjectDisposedException(GetType().Name);
    }
}