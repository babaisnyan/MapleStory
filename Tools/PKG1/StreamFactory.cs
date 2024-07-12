using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace PKG1;

public class StreamFactory
{
    private readonly Func<Stream> _createNew;
    private volatile ConcurrentBag<StreamContainer> _containers;
    private readonly Timer _timedThread;
    private int _disposed;

    public StreamFactory(Func<Stream> createNew)
    {
        _createNew = createNew;
        _containers = new ConcurrentBag<StreamContainer>();
        _timedThread = new Timer(ContainerWatcher, null, TimeSpan.FromSeconds(10), TimeSpan.FromSeconds(10));
    }

    public Stream GetStream()
    {
        if (_disposed != 0) throw new ObjectDisposedException("StreamFactory");
        var threadId = Thread.CurrentThread.ManagedThreadId;

        var res = _containers.FirstOrDefault(c => c.Lock(threadId));

        if (res == null)
        {
            res = new StreamContainer(_createNew());
            res.Lock(threadId);
            _containers.Add(res);
        }

        return res;
    }

    internal void Dispose()
    {
        var waitForDisposed = new EventWaitHandle(false, EventResetMode.ManualReset);
        _timedThread.Dispose(waitForDisposed);
        waitForDisposed.WaitOne();

        _disposed = 1;
        var containersDisposing = Interlocked.CompareExchange(ref _containers, null, _containers);
        if (containersDisposing != null)
            foreach (var container in containersDisposing.Where(c => c.Lock(-1)))
                container.Dispose();
    }

    private void ContainerWatcher(object state)
    {
        var oldContainers = _containers;

        if (oldContainers.Count > 0 && _disposed == 0)
        {
            var now = DateTime.Now;
            IEnumerable<StreamContainer> disposing = oldContainers.Where(c => _disposed == 0 && (now - c._lastLock).Seconds > 10 && !c.IsLocked && c.Lock(-1)).ToArray();

            if (_disposed == 0 && disposing.Count() > 0)
            {
                _containers = new ConcurrentBag<StreamContainer>(
                    oldContainers.Where(c => !disposing.Contains(c))
                );
                Parallel.ForEach(disposing, c =>
                {
                    c.Dispose();
                    c._underlying.Dispose();
                });
            }

            while (oldContainers.TryTake(out var blah)) ;
        }
    }

    private class StreamContainer : Stream, IDisposable
    {
        public DateTime _lastLock;
        internal readonly Stream _underlying;
        private volatile int _locked;

        public StreamContainer(Stream s)
        {
            _underlying = s;
        }

        public override bool CanRead => _underlying.CanRead;
        public override bool CanSeek => _underlying.CanSeek;
        public override bool CanWrite => _underlying.CanWrite;
        public override long Length => _underlying.Length;

        public override long Position
        {
            get => _underlying.Position;
            set
            {
                if (_locked != Thread.CurrentThread.ManagedThreadId) throw new InvalidOperationException("Modifying a stream container that the thread doesn't have control over");
                _underlying.Position = value;
            }
        }

        public bool IsLocked => _locked != 0;

        public override void Flush()
        {
            _underlying.Flush();
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            if (_locked != Thread.CurrentThread.ManagedThreadId) throw new InvalidOperationException("Modifying a stream container that the thread doesn't have control over");
            return _underlying.Read(buffer, offset, count);
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            if (_locked != Thread.CurrentThread.ManagedThreadId) throw new InvalidOperationException("Modifying a stream container that the thread doesn't have control over");
            return _underlying.Seek(offset, origin);
        }

        public override void SetLength(long value)
        {
            _underlying.SetLength(value);
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            _underlying.Write(buffer, offset, count);
        }

        public bool Lock(int threadId)
        {
            if (!_underlying.CanRead) return false;
            var exchangedLock = Interlocked.CompareExchange(ref _locked, threadId, 0);
            var gotLock = exchangedLock == 0;
            if (gotLock) _lastLock = DateTime.Now;
            return gotLock;
        }

        protected override void Dispose(bool disposing)
        {
            var threadId = Thread.CurrentThread.ManagedThreadId;
            var originalLock = _locked;
            if (originalLock != Thread.CurrentThread.ManagedThreadId && originalLock != -1 && originalLock != 0) throw new InvalidOperationException("Unlocking a stream container that the thread doesn't have control over");
            _locked = 0;
            if (disposing && originalLock == -1)
                _underlying.Dispose();
        }
    }
}