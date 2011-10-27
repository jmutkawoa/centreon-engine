#ifndef CCE_COMMANDS_BASIC_PROCESS_HH
# define CCE_COMMANDS_BASIC_PROCESS_HH

# include <QIODevice>
# include <QString>
# include <QStringList>
# include <QByteArray>
# include <QProcess>
# include <QSocketNotifier>
# include <QSharedPointer>

namespace                          com {
  namespace                        centreon {
    namespace                      engine {
      namespace                    commands {
        /**
         *  @class basic_process commands/basic_proces.hh
         *  @brief Basic process is a reimplementation of QProcess
         *  without use select function.
         *
         *  Basic process provide a simple interface to execute
         *  some process.
         */
        class                      basic_process : public QIODevice {
          Q_OBJECT
        public:
          basic_process(QObject* parent = 0);
          virtual ~basic_process() throw();

          void                     closeReadChannel(QProcess::ProcessChannel channel);
          void                     closeWriteChannel();
          // QStringList              environment() const; // (deprecated)
          QProcess::ProcessError   error() const;
          int                      exitCode() const;
          QProcess::ExitStatus     exitStatus() const;
          // QString                  nativeArguments() const;
          Q_PID                    pid() const;
          // ProcessChannelMode       processChannelMode() const;
          // QProcessEnvironment      processEnvironment() const;
          QByteArray               readAllStandardError();
          QByteArray               readAllStandardOutput();
          QProcess::ProcessChannel readChannel() const;
          // void                     setEnvironment(QStringList const& environment); // (deprecated)
          // void                     setNativeArguments(QString const& arguments);
          // void                     setProcessChannelMode(ProcessChannelMode mode);
          // void                     setProcessEnvironment(QProcessEnvironment const& environment);
          void                     setReadChannel(QProcess::ProcessChannel channel);
          // void                     setStandardErrorFile(QString const& fileName, OpenMode mode = Truncate);
          // void                     setStandardInputFile(QString const& fileName);
          // void                     setStandardOutputFile(QString const& fileName, OpenMode mode = Truncate);
          // void                     setStandardOutputProcess(QProcess* destination);
          void                     setWorkingDirectory(QString const& dir);
          void                     start(QString const& program, QStringList const& arguments, OpenMode mode = ReadWrite);
          void                     start(QString const& program, OpenMode mode = ReadWrite);
          QProcess::ProcessState   state() const;
          bool                     waitForFinished(int msecs = 30000);
          bool                     waitForReadyRead(int msecs = 30000);
          bool                     waitForBytesWritten(int msecs = 30000);
          bool                     waitForStarted(int msecs = 30000);
          QString                  workingDirectory() const;

          // QIODevice
          qint64                   bytesAvailable() const;
          qint64                   bytesToWrite() const;
          bool                     isSequential() const;
          bool                     canReadLine() const;
          void                     close();
          bool                     atEnd() const;

        signals:
          void                     error(QProcess::ProcessError error);
          void                     finished(int exitCode, QProcess::ExitStatus exitStatus);
          void                     readyReadStandardError();
          void                     readyReadStandardOutput();
          void                     started();
          void                     stateChanged(QProcess::ProcessState newState);

        public slots:
          void                     kill();
          void                     terminate();

        protected:
          void                     setProcessState(QProcess::ProcessState state);
          virtual void             setupChildProcess();

          // QIODevice
          virtual qint64           readData(char* data, qint64 maxlen);
          virtual qint64           writeData(char const* data, qint64 len);

        private slots:
          void                     _notification_standard_output();
          void                     _notification_standard_error();
          void                     _notification_dead();

        private:
          basic_process(basic_process const&);
          basic_process& operator=(basic_process const&);

          void                     _start_process(OpenMode mode);
          void                     _exec_child();
          void                     _close_pipe() throw();
          void                     _emit_finished();
          static bool              _read(int fd, QByteArray* str);
          static qint64            _available_bytes(int fd) throw();
          static pid_t             _waitpid(pid_t pid, int* status, int options) throw();
          static qint64            _read(int fd, void* buf, qint64 nbyte) throw();
          static void              _close(int& fd) throw();
          static int               _chdir(char const* working_directory) throw();
          static int               _dup2(int fildes, int fildes2) throw();
          static char**            _build_args(QString const& program, QStringList const& arguments);
          static QStringList       _split_command_line(QString const& command_line);

          // QProcessEnvironment      _environment;
          QStringList              _arguments;
          QString                  _program;
          QString                  _working_directory;
          QByteArray               _standard_output;
          QByteArray               _standard_error;
          QSocketNotifier*         _notifier_output;
          QSocketNotifier*         _notifier_error;
          QSocketNotifier*         _notifier_dead;
          QProcess::ProcessChannel _channel;
          QProcess::ProcessError   _perror;
          QProcess::ProcessState   _pstate;
          pid_t                    _pid;
          int                      _pipe_out[2];
          int                      _pipe_err[2];
          int                      _pipe_in[2];
          int                      _pipe_dead[2];
          int                      _status;
        };
      }
    }
  }
}

#endif // !CCE_COMMANDS_BASIC_PROCESS_HH