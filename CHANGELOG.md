# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

## [0.0.5] - 2022-06-02
### Fixed
- Removed extra mutex to avoid condition variable miss in event loop.

## [0.0.4] - 2022-06-02
### Fixed
- Deadlock in EventDispatcher when getInstance() and lockAttachments called at 
the same time.

## [0.0.3] - 2022-05-16
### Changed
- Code style.

## [0.0.2] - 2022-04-21
### Added
- Object(Object* parent = nullptr) to make it easier to attach to the event 
loop.

## [0.0.1] - 2022-04-19
### Added
- Sources :)

[0.0.5]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.5
[0.0.4]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.4
[0.0.3]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.3
[0.0.2]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.2
[0.0.1]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.1